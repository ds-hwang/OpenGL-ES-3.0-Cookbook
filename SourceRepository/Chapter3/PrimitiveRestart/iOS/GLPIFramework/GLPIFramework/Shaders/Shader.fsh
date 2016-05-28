//
//  Shader.fsh
//  GLPIFramework
//
//  Created by macbook on 9/8/13.
//  Copyright (c) 2013 macbook. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
