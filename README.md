# CaerulusEngine
Entity Component System Game Engine

A game engine I have been developing in my spare time to practise C++.

One of the design decisions was not to document any of the code and instead rely upon concise, self-documenting code.

![alt text](https://github.com/JustinRJ/CaerulusEngine/blob/main/sponza.png?raw=true)


# Notes

The engine makes use of RTTI over polymorphism wherever possible.
This reduces the virtual dispatches required as well as removing the need to dynamic_cast, although it slightly increases the size of the binaries.

The Entity class is used to represent a scene as a tree hierarchy; each entity is composed of (none-many) Components.
Components can be created and attached to Entities at any given time, the Components will always be created sequentially on the heap.

Physically Based Rendering techniques are utilized.
The engine is designed to allow any 

# Improvements

    * Integrate Scene class into sub-systems instead of passing tickables & entities separately

    * Networking is currently just a skeleton, wrap char** in struct with socket info to send when char** buffer is full

    * Graphics is old opengl, re-write to use Vulkan (this will be large undertaking)

    * Split Core into CoreLib and Core, Core will contain state while CoreLib will be purely functionality based

    * Split Engine.cpp into a new .sln called Game (larger divide between Engine and Game)

    * AssetManagers can make use of varadiac templates to have a consistent Load(...) interface