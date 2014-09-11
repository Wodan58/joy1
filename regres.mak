regres: flatjoy grmtst joytut jp-church jp-joytst jp-nestrec jp-reprodtst laztst lsptst modtst mtrtst plgtst reptst symtst test
	rm -f joy.out

flatjoy:
	./joy $@.joy | diff -w - $@.out

grmtst:
	./joy $@.joy | sed '/^warning/d' >joy.out
	tail --lines=+3 $@.out | head --lines=328 | diff -w - joy.out

joytut:
	./joy $@.inp </dev/null >joy.out
	sed '/^JOY/d;/^Copy/d;/^Monday/d;/^time/d;s/joytut/joytut quit/' <$@.out | diff -w - joy.out

jp-church:
	./joy $@.joy | diff -w - $@.out

jp-joytst:
	./joy $@.joy | sed '/^warning/d' >joy.out
	sed '/^JOY/d;/^Copy/d;/^Thursday/d;/^time/d;/^gc/d' <$@.out | diff -w - joy.out

jp-nestrec:
	./joy $@.joy | diff -w - $@.out

jp-reprodtst:
	./joy $@.joy | diff -w - $@.out

laztst:
	./joy $@.joy >joy.out
	sed '/^JOY/d;/^Copy/d;/^Friday/d;/^Time/d' <$@.out | diff -w - joy.out

lsptst:
	./joy $@.joy </dev/null >joy.out
	head -265 <$@.out | sed '/^JOY/d;/^Copy/d;s/^lisp/lisp quit/' | diff -w - joy.out

modtst:
	./joy $@.joy >joy.out
	tail --lines=+3 $@.out | diff -w - joy.out

mtrtst:
	./joy $@.joy >joy.out
	tail --lines=+3 $@.out | diff -w - joy.out

plgtst:
	./joy $@.joy >joy.out
	tail --lines=+3 $@.out | diff -w - joy.out

reptst:
	./joy $@.joy | diff -w - $@.out

symtst:
	./joy $@.joy >joy.out
	tail --lines=+3 $@.out | diff -w - joy.out

test:
	./joy $@.joy | diff -w - $@.out
