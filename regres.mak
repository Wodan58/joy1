regres: gcd24_54 modtst test joytut jp-reprodtst mtrtst flatjoy jp-church reptst symtst jp-joytst plgtst lsptst laztst jp-nestrec grmtst
	rm -f joy.out

gcd24_54:
	./joy gcd 24 54 | diff -w - gcd.out

joytut:
	./joy $@.inp >joy.out
	sed '/^JOY/d;/^Copy/d;/^Monday/d;/^time/d;s/joytut/joytut quit/' <$@.out | diff -w - joy.out

lsptst:
	./joy $@.joy | tail -259 | head -255 >joy.out
	head -261 <$@.out | tail -255 | sed '/^JOY/d;/^Copy/d;s/^lisp/lisp quit/' | diff -w - joy.out

jp-joytst:
	./joy $@.joy | sed '/^warning/d' >joy.out
	sed '/^JOY/d;/^Copy/d;/^Thursday/d;/^time/d;/^gc/d' <$@.out | diff -w - joy.out

laztst:
	./joy $@.joy >joy.out
	sed '/^JOY/d;/^Copy/d;/^Friday/d;/^Time/d' <$@.out | diff -w - joy.out

grmtst:
	./joy $@.joy | sed '/^warning/d' >joy.out
	tail --lines=+3 $@.out | head --lines=328 | diff -w - joy.out

modtst:
	./joy $@.joy >joy.out
	tail --lines=+3 $@.out | diff -w - joy.out

mtrtst:
	./joy $@.joy >joy.out
	tail --lines=+3 $@.out | diff -w - joy.out

plgtst:
	./joy $@.joy >joy.out
	tail --lines=+3 $@.out | diff -w - joy.out

symtst:
	./joy $@.joy >joy.out
	tail --lines=+3 $@.out | diff -w - joy.out

test:
	./joy $@.joy | diff -w - $@.out

reptst:
	./joy $@.joy | diff -w - $@.out

jp-nestrec:
	./joy $@.joy | diff -w - $@.out

jp-reprodtst:
	./joy $@.joy | diff -w - $@.out

flatjoy:
	./joy $@.joy | diff -w - $@.out

jp-church:
	./joy $@.joy | diff -w - $@.out
