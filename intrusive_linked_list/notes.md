I think I fucked up the implementation of this on the first attempt.

Dereferencing an iterator **must** yield a reference to a value for which we
have created the linked list. In my current implementation, dereferencing an
iterator instead yields a `list_hook` reference. BAD!

The sentinel nodes acts as the `one past the end` iterator. A list is circular
and thus one never runs the risk of dereferencing a `nullptr`.

The bidirectional iterator facade requires the following template parameters:
* Derived (return value for unary increment, etc.)
* Stored type (the type of the parent of the list hook member)
* Hook (the pointer to the list hook member)
Given this, an increment / decrement can jump to the next list hook.
Equality is given by comparing the address of the list hooks.
Dereferencing can return an instance of the stored type.
