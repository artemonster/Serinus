Some useful links on dynamic prototypes:

http://www.cs.sjsu.edu/~pearce/modules/lectures/oop/types/reflection/prototype.htm
http://stackoverflow.com/questions/582331/is-there-a-way-to-instantiate-objects-from-a-string-holding-their-class-name

Generally, I like the idea of registration, but this is too resource costly on the embedded system.
I think we can either map all available classes in a hashmap OR we just can use a manually-made pointer table