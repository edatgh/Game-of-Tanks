
#ifndef _TANK__GRAPHICS_H_
#define _TANK__GRAPHICS_H_

/*=INCLUDES=*/
#include "tgincludes.h"

/*=Model class definition=*/
class Model {
public:
    void Load(char *filename); // Loads a model from the given file
    void Draw(int x,int y);
private:
    Pixmap m_ID; // ID of the model
};

#endif
