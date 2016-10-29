regres: gcd24_54 modtst test joytut jp-reprodtst mtrtst flatjoy jp-church reptst symtst jp-joytst plgtst lsptst laztst jp-nestrec grmtst
	rm -f joy.out

gcd24_54:
	./joy gcd 24 54 | diff -w - gcd.out

joytut:
	./joy $@.inp >joy.out
	sed '/^Monday/d;/^time/d;s/^joytut/joytut quit/' <$@.out | diff -w - joy.out

lsptst:
	./joy $@.joy | head -261 >joy.out
	head -261 <$@.out | sed 's/^lisp/lisp quit/' | diff -w - joy.out

jp-joytst:
	./joy $@.joy | sed '/^warning/d' >joy.out
	sed '/^Thursday/d;/^time/d;/^gc/d' <$@.out | diff -w - joy.out

laztst:
	./joy $@.joy >joy.out
	sed '/^Friday/d;/^Time/d' <$@.out | diff -w - joy.out

grmtst:
	./joy $@.joy >joy.out
	head -330 $@.out | diff -w - joy.out

modtst:
	./joy $@.joy | diff -w - $@.out

mtrtst:
	./joy $@.joy | diff -w - $@.out

plgtst:
	./joy $@.joy | diff -w - $@.out

symtst:
	./joy $@.joy | diff -w - $@.out

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
