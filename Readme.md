## MFC Learning Notes
Code examples are from the book Programming Windows with MFC by Jeff Prosise.

Examples have been updated to work with VS2022.

If you are starting from an empty C++ and building the app from the first principles, make sure to
set the appropriate configuration and linker settings as follows:

1. Project Properties->Linker->System->SubSystem is set to Windows
2. Project Properties ->Advanced->Use of MFC ->Use MFC in a shared DLL (use of static would work as well but would create larger binaries)

If you get any Access Denied errors trying to run the executables, this is probably the antivirus for which you would need to setup an exception.
