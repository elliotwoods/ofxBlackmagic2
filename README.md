ofxBlackmagic2
==============

Advantages:

* Uses DeckLink SDK for colour conversion (much faster/more efficient than hand-rolled solutions)
* Designed for use with multiple devices
* Object oriented with namespaces
* Lockable frames for threading

Alternatives:

* http://github.com/jamezilla/ofxBlackmagic 
* https://github.com/kylemcdonald/ofxBlackmagic

Installation
------------

This addon has been tested on Windows, but it should also work on OSX and Linux (by including the DeckLink SDK's for those platforms in your project and ignoring DeckLinkAPI_i.c, DeckLinkAPI_i.h).

On Windows, install BlackMagic Desktop Video (not the SDK), then try out an example.

### Visual Studio 2012

#### Remove CompileAsCpp

By default (as of 30-09-2013, oF 008), openFrameworks projects are created with a specific flag turned on called `CompileAsCpp`. To use the DeckLink API, you need to turn this flag off. I've done this for you in the examples, so feel free to just grab one of the example projects and run from there.

If you want to add ofxBlackmagic2 to an existing project, then you need to do the following steps:

1. Right click on your app project in Visual Studio and select 'Properties'.
2. In the `<u>C</u>onfiguration` drop down box select `All Configurations` (this means that you're editing Debug and Release at the same time).
3. Browse the tree on the left to `Configuration Properties\ C/C++ \ Advanced`
4. Change the property `CompileAs` so that it reads `Default`
5. Hit `OK` to save

If you don't make this change, then it's highly likely that you'll experience linker errors (e.g. something like 'undefined symbols'). See http://forum.blackmagicdesign.com/viewtopic.php?f=10&t=13195 for more details.

#### Using ofxBlackmagicLib pattern

ofxBlackmagic2 follows the pattern of 'one project and one namespace per sizeable addon' pattern. This means that the best way of using ofxBlackmagic2 is __NOT__ to put the files from  `ofxBlackmagic2/src` into your app project, but to add the `ofxBlackmagicLib/ofxBlackmagicLib.vcxproj` project to your app's solution, and to add __only__ the `ofxBlackmagic2/src` to your include search path.

To do this follow these steps:

1. Right click on your solution and choose `Add`\`Existing Project...`
2. Select `ofxBlackmagic2/ofxBlackmagicLib/ofxBlackmagicLib.vcxproj`
3. Right click on your app project and select `Properties`
4. Go to `Common Properties` at the top of the tree on the left
5. Hit the `Add <u>N</u>ew Reference` button and select `ofxBlackmagicLib`
6. Hit `OK` to save

You then must also go to the project properties of your app, go to `Common Properties` on the left hand side, 


License
-------

MIT License
http://opensource.org/licenses/MIT
