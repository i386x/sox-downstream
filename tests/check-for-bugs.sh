
. $(dirname $0)/lib/errno

declare -A TESTCASES

TESTCASES=(
  [1500554]="It is a heap-buffer-overflow in ImaExpandS (in ima_rw.c:126)"
  [1500554-CVE]="2017-15370"
  [1500554-requires]="input"
  [1500554-input]="data/02-heap-buffer-over"
  [1500554-output]="/var/tmp/tt.snd"
  [1500554-cmd]="sox \${TESTCASES[1500554-input]} \${TESTCASES[1500554-output]:-/var/tmp/foo}"
  [1500554-exitcode-if-fixed]=$EXIT_SUCCESS
  [1500554-exitcode-if-not-fixed]=$EXIT_SEGV
  [1500554-teardown]="rm -f \${TESTCASES[1500554-output]:-/var/tmp/foo}"

  [1500570]="It is a reachable assertion abort in function sox_append_comment(in formats.c:227) that will lead to denial of service attack"
  [1500570-CVE]="2017-15371"
  [1500570-requires]="input"
  [1500570-input]="data/03-abort"
  [1500570-cmd]="sox \${TESTCASES[1500570-input]} /dev/null"
  [1500570-exitcode-if-fixed]=$EXIT_EINPUT
  [1500570-exitcode-if-not-fixed]=$EXIT_ABRT
)

SUCCEEDS=0
FAILURES=0

succeed() {
  SUCCEEDS=$((SUCCEEDS + 1))
  echo "[PASSED]"
}

failure() {
  local T

  T=0
  [ "$1" = "-p" ] && { T=1; shift; }

  FAILURES=$((FAILURES + 1))
  if [ $T -eq 0 ]; then
    echo "~> $1"
    echo "[FAILED]"
  else
    echo "$1"
  fi
}

check_for_bug() {
  local C
  local E

  E=$EXIT_SUCCESS
  if [ -z "${TESTCASES[$1]}" ]; then
    failure -p "No test case for #$1"
    return $EXIT_FAILURE
  fi
  # Welcome message:
  echo -n "Checking if RHBZ bug #$1 \"${TESTCASES[$1]}\""
  if [ -z "${TESTCASES[$1-CVE]}" ]; then
    echo -n " (CVE-${TESTCASES[$1-CVE]})"
  fi
  echo " is not present:"
  # Setup:
  if [ "${TESTCASES[$1-setup]}" ]; then
    C="${TESTCASES[$1-setup]}"; eval "C=\"$C\""
    eval "$C"; E=$?
    if [ $E -ne 0 ]; then
      failure "$1: Setup has failed!"
      return $EXIT_FAILURE
    fi
  fi
  # Requirements:
  if [ "${TESTCASES[$1-requires]}" ]; then
    for x in ${TESTCASES[$1-requires]}; do
      if [ -z "${TESTCASES[$1-$x]}" ]; then
        failure "TESTCASES: bad key $1-$x"
        return $EXIT_FAILURE
      fi
      if [ ! -f "$(dirname $0)/${TESTCASES[$1-$x]}" ]; then
        failure "$1: Missing $(dirname $0)/${TESTCASES[$1-$x]}!"
        return $EXIT_FAILURE
      fi
    done
  fi
  # Test case prerequisities:
  if [ -z "${TESTCASES[$1-cmd]}" ]; then
    failure "$1: Missing command!"
    return $EXIT_FAILURE
  fi
  if [ -z "${TESTCASES[$1-exitcode-if-fixed]}" ]; then
    failure "$1: Missing exit code for fixed bug case!"
    return $EXIT_FAILURE
  fi
  if [ -z "${TESTCASES[$1-exitcode-if-not-fixed]}" ]; then
    failure "$1: Missing exit code for not fixed bug case!"
    return $EXIT_FAILURE
  fi
  # Execute command:
  C="${TESTCASES[$1-cmd]}"; eval "C=\"$C\""
  (cd $(dirname $0); eval "$C"); E=$?
  # Check exit code:
  if [ $E -eq ${TESTCASES[$1-exitcode-if-not-fixed]} ]; then
    failure "$1: NOT FIXED!"
    return $EXIT_FAILURE
  elif [ $E -eq ${TESTCASES[$1-exitcode-if-fixed]} ]; then
    : "FIXED!"
  else
    failure "$1: Unexpected exit code $E!"
    return $EXIT_FAILURE
  fi
  # Tear down:
  if [ "${TESTCASES[$1-teardown]}" ]; then
    C="${TESTCASES[$1-teardown]}"; eval "C=\"$C\""
    eval "$C"; E=$?
    if [ $E -ne 0 ]; then
      failure "$1: Tear down has failed!"
      return $EXIT_FAILURE
    fi
  fi
  succeed
  return $EXIT_SUCCESS
}

results() {
  local A

  A=$((SUCCEEDS + FAILURES))
  echo ""
  echo "SUCCEEDS: $SUCCEEDS"
  echo "FAILURES: $FAILURES"
  echo "TOTAL:    $A"
  echo ""
  if [ $FAILURES -ne 0 ]; then
    return $EXIT_FAILURE
  fi
  return $EXIT_SUCCESS
}

check_for_bug 1500554
check_for_bug 1500570
results
