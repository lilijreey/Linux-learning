OBJECTS= tuple array varTemplateParameter initializer_list constexpr \
	 rightValueReference lambda deleteControl regex chrono function other

all: $(OBJECTS)

# -O 优化
O:=-O$(o) #immediately assigment

define compile-and-run
	@g++ -Wall $(O) -std=c++11 $< -o $@
	@./$@
endef

tuple: tuple.cpp
	$(compile-and-run)

array: array.cpp
	$(compile-and-run)

varTemplateParameter: varTemplateParameter.cpp
	$(compile-and-run)
	
initializer_list: initializer_list.cpp
	$(compile-and-run)

lambda: lambda.cpp
	$(compile-and-run)

constexpr: constexpr.cpp
	$(compile-and-run)

nullptr: nullptr.cpp
	$(compile-and-run)

for-loop-range-based: for-loop-range-based.cpp
	$(compile-and-run)

rightValueReference: rightValueReference.cpp
	$(compile-and-run)

unordered: unordered.cpp
	$(compile-and-run)

deleteControl: deleteControl.cpp
	$(compile-and-run)

regex: regex.cpp
	$(compile-and-run)

chrono: chrono.cpp
	$(compile-and-run)

function: function.cpp
	$(compile-and-run)

other: othersFeature.cpp
	$(compile-and-run)

alg: algorithms.cpp
	$(compile-and-run)

clean:
	@rm -rf $(OBJECTS)

#for git command

COMMIT_MSG=$(m)
RC := git
commit:
	$(RC) commit -a -m "$(COMMIT_MSG)"

push:
	$(RC) push origin master

st:
	$(RC) status

.PHONY: commit push all st

