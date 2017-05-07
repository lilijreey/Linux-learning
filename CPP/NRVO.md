
-fno-elide-constructors

The C++ standard allows an implementation 
to omit creating a temporary that is only
used to initialize another object of 
the same type.
Specifying this option disables that 
optimization, and forces G++ to
call the copy constructor in all cases.
