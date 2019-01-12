add_custom_target(test01.txt ALL
		  DEPENDS joy
		  COMMAND joy test01.joy >test01.txt)
