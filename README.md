This file is part of SoarUI.  
Soar: the simulate window system and do window logic   
SoarEngine:UI resource manager,it holds all the UI resources ; the Coordinate system came from CEGUI library. see http://cegui.org.uk/  
SoarHeader:the SDK of interface ,in early times ,i thought i will public it as sdk ,but now ,public it as source.   
SoarProvider: a plugin to load UI and texture resource.  
SoarRender： a plugin to render the ui in Direct system.  
SoarTest: a sample for test  
StdexpatParser: a XML handler using expat.see https://github.com/libexpat/libexpat  
using SoarUI you can add anything in the window anywhere and do not want to derive from base class. the UI(I mean as lookNFeel/skin) and the logic is separated. you can change the window skin by using API or write your own logic for it by reggisting your handle functions. if you need more detail for skin ,you can using the looknfeel file(.flydream),just like the cegui looknfeel(i had modify some files to fix my mind)   
the Imageset is the same as CEGUI.you can use its image tool to do your images.  
- 📫 How to reach me: nicklisir@gmail.com
