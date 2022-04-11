
#ifndef _Imageset_xmlHandler_h_
#define _Imageset_xmlHandler_h_

#include "../../SoarHeader/LeeString.h"
#include "../Imageset.h"
#include "../../XML/XMLAttributes.h"
#include "../../XML/IXMLHandler.h"


class  Imageset_xmlHandler : public IXMLHandler
{
public:
    //! Constructor.
    Imageset_xmlHandler(void);

    //! Destructor.
    ~Imageset_xmlHandler();

    //! Return string holding the name of the created Imageset.
    const CLeeString& getObjectName() const;

    //! Return reference to the created Imageset object.
    Imageset* getObject() const;

    // XMLHandler overrides
    void elementStart(const CLeeString& element, const XMLAttributes& attributes);
    void elementEnd(const CLeeString& element);
    void text(const CLeeString & text){}
private:
    //! Method that handles the opening Imageset XML element.
    void elementImagesetStart(const XMLAttributes& attributes);
    //! Method that handles the Image XML element.
    void elementImageStart(const XMLAttributes& attributes);
    //! Method that handles the closing Imageset XML element.
    void elementImagesetEnd();
    //! Tag name for Imageset elements.
    static const CLeeString ImagesetElement;
    //! Tag name for Image elements.
    static const CLeeString ImageElement;
    //! Attribute name that stores the name of the Imageset
    static const CLeeString ImagesetNameAttribute;
    //! Attribute name that stores the filename for the image file.
    static const CLeeString ImagesetImageFileAttribute;
    //! Attribute that stores 'native' horizontal resolution for the Imageset.
    static const CLeeString ImagesetNativeHorzResAttribute;
    //! Attribute that stores 'native' vertical resolution for the Imageset.
    static const CLeeString ImagesetNativeVertResAttribute;
    //! Attribute that specifies whether the Imageset should be auto-scaled.
    static const CLeeString ImagesetAutoScaledAttribute;
    //! Attribute name that stores the name of the new Image.
    static const CLeeString ImageNameAttribute;
    //! Attribute name that stores the x position of the new Image.
    static const CLeeString ImageXPosAttribute;
    //! Attribute name that stores the y position of the new Image.
    static const CLeeString ImageYPosAttribute;
    //! Attribute name that stores the width of the new Image.
    static const CLeeString ImageWidthAttribute;
    //! Attribute name that stores the height of the new Image.
    static const CLeeString ImageHeightAttribute;
    //! Attribute name that stores the x rendering offset of the new Image.
    static const CLeeString ImageXOffsetAttribute;
    //!< Attribute name that stores the y rendering offset of the new Image.
    static const CLeeString ImageYOffsetAttribute;

    //! Pointer to the Imageset created.
    Imageset* d_imageset;
    //! inidcates whether client read the created object
    mutable bool d_objectRead;
};


#endif  // end of guard _Imageset_xmlHandler_h_
