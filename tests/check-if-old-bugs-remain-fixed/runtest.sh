
here=$(pwd)/$(dirname $0)

. /usr/share/beakerlib/beakerlib.sh

TEST="/tests/check-if-old-bugs-remain-fixed"

PACKAGE="sox"

rlJournalStart
  rlPhaseStartSetup
    rlRun "DataDir=$here/data" 0 'Obtaining path to data directory'
    rlRun "SndFile=/var/tmp/temp$$.snd" 0 'Generating temporary .snd file name'
    rlRun "TmpFile=/var/tmp/temp$$" 0 'Generating temporary file name'
  rlPhaseEnd

  rlPhaseStartTest
    rlRun "soxdbg $DataDir/02-heap-buffer-over $SndFile" 0 'Test for CVE-2017-15370 presence'
    rlRun "soxdbg $DataDir/03-abort /dev/null" 2 'Test for CVE-2017-15371 presence' 2>&1 | tee $TmpFile
    rlAssertGrep 'FLAC ERROR whilst decoding metadata' $TmpFile
    rlRun "soxdbg $DataDir/01-stack-overflow $SndFile" 0 'Test for CVE-2017-15372 presence'
    rlRun "soxdbg -D -V -V $DataDir/crash00 /dev/null" 2 'Test for CVE-2017-15642 presence'
  rlPhaseEnd

  rlPhaseStartCleanup
    rlRun "rm -f $SndFile $TmpFile" 0 "Removing temporary files"
  rlPhaseEnd
rlJournalEnd

rlJournalPrintText
