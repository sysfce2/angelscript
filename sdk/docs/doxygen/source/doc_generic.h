/**

\page doc_generic The generic calling convention

The generic calling convention is available for those situations where the application's native calling 
convention doesn't work, for example on platforms where support for native calling conventions haven't 
been added yet. You can detect if native calling conventions isn't supported on your target platform by
calling the \ref asGetLibraryOptions function and checking the returned string for "AS_MAX_PORTABILITY". If the
identifier is in the returned string, then native calling conventions is not supported.

Functions implementing the generic calling conventions are always global functions (or static class methods), 
that take as parameter a pointer to an asIScriptGeneric interface and returns void.

\code
// The function has been registered with signature:
//  MyIntf @func(int, float, MyIntf @+)
void MyGenericFunction(asIScriptGeneric *gen)
{
  // Extract the arguments
  int arg0              = gen->GetArgDWord(0);
  float arg1            = gen->GetArgFloat(1);
  asIScriptObject *arg2 = reinterpret_cast<asIScriptObject*>(gen->GetArgObject(2));
  
  // Call the real function
  asIScriptObject *ret = MyFunction(arg0, arg1, arg2);
  
  // Set the return value
  gen->SetReturnObject(ret);
}
\endcode

Functions using the generic calling convention can be registered anywhere the script engine is expecting 
global functions or class methods (except where explicitly written otherwise). 

Writing the functions for the generic calling convention requires extracting each argument from the AngelScript
stack, and then manually giving the return value back. For that reason it may be desired to use the 
\ref doc_addon_autowrap "automatic wrapper functions" rather than writing the functions yourself.

\section doc_generic_1 Extracting function arguments

To extract functions arguments from the generic interface you should call one of the GetArg methods that
will return the value of the argument, or the \ref asIScriptGeneric::GetAddressOfArg "GetAddressOfArg" method. 
The GetAddressOfArg method returns a pointer to the actual value. The application should then cast this pointer 
to a pointer of the correct type so that the value can be read from the address.

If the function you're implementing represents a class method, the pointer to the object instance should be obtained 
with a call to \ref asIScriptGeneric::GetObject "GetObject".

For arguments that receive output references, i.e. &amp;out, the GetAddressOfArg will give an address to a valid instance of the type.
If the function does not want to return anything in these arguments it is not necessary to do anything.

The asIScriptGeneric interfaces treats the ref count the same way as in native calling conventions, i.e. the application
function is responsible to releasing any handle received as a handle (or \ref doc_obj_handle_4 "register the function with @+").

\section doc_generic_2 Returning values

To return a value from the function one of the SetReturn methods can be called to pass the value to the 
generic interface. Returning primitive values is straight forward, but care must be taken when returning object types,
either by value, reference, or as object handle. Depending on the type and the function used it may be necessary to
increment the reference count, or even make a copy of the object first. Carefully read the instructions for 
\ref asIScriptGeneric::SetReturnAddress "SetReturnAddress" and \ref asIScriptGeneric::SetReturnObject "SetReturnObject" 
to determine what needs to be done to get the expected result.

It is also possible to use the \ref asIScriptGeneric::GetAddressOfReturnLocation "GetAddressOfReturnLocation" method to
obtain the address of the memory where the return value will be stored. The memory is not initialized, so you should
use the placement new operator to initialize this memory with a call to the constructor. This also works for primitive
types, which makes this ideal for template implementations, such as that in the \ref doc_addon_autowrap "automatic wrapper functions".

If the return type is a handle then the GetAddressOfReturnLocation will refer to a location holding a null pointer, so if the function 
is to return null it is not necessary to do anything. For primitives it will be an undefined value, so if the value is important it must be set.
For objects of value types returned by value the function must initialize the return value at the location already preallocated by the 
calling function, unless an exception is set with \ref asIScriptContext::SetException.

If the function is registered to return a @+, then the script engine will \ref doc_obj_handle_4 "automatically increment the ref count", just as is
done for native calling conventions.


*/
