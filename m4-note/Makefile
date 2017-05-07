all: code.lua monster.lua

code.lua: m4-generate-code-template.lua.m4
	rm -f $@
	m4 $^ > $@

monster.lua: monster.lua.m4
	rm -f $@
	m4 $^ > $@

test:
	m4 test.m4
