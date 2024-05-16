.global start, testing
.section testsec

start:      halt
            bgt %r1, %r2, start
main:       int
            call [ %r1  + 123 ]
        

.section    data
testing:    


.skip 0x1
.extern testtwo
            int
            halt
skip:       int
.end