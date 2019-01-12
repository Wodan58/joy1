add_custom_target(test01.txt ALL
		  DEPENDS joy
		  COMMAND joy test01.joy >test01.txt)
add_custom_target(test02.txt ALL
		  DEPENDS joy
		  COMMAND joy test02.joy >test02.txt)
add_custom_target(test03.txt ALL
		  DEPENDS joy
		  COMMAND joy test03.joy >test03.txt)
add_custom_target(test04.txt ALL
		  DEPENDS joy
		  COMMAND joy test04.joy >test04.txt)
add_custom_target(test05.txt ALL
		  DEPENDS joy
		  COMMAND joy test05.joy test05.joy >test05.txt)
