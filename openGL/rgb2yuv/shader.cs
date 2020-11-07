#extension GL_NV_image_formats : enable
layout (rgba8, binding = 0) readonly uniform lowp image2D rgbaImage;
layout (r8, binding = 1) writeonly uniform lowp image2D yImage;
layout (local_size_x = 16, local_size_y = 16) in;
layout(location = 2) uniform int height;
void main()
{                                                                                                            
        ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
        ivec2 nvPos;
        ivec2 ypos;
        vec4 vr;
        vec4 vl;
        vec4 vld;
        vec4 vrd;
        vec4 sumUV;
        vec4 yvec;
        float y0,u0,v0,y1,u1,v1,y2,u2,v2,y3,u3,v3;
	if(storePos.y >= height)
	{
		return;
	}
        if(storePos.x % 2 == 0 && storePos.y % 2 == 0) 
        {                                                  
		nvPos = storePos;
		ypos = storePos;                           
	        vl = imageLoad(rgbaImage, storePos);             
	        storePos.x+=1;                                 
	        vr = imageLoad(rgbaImage, storePos);             
	        storePos.y+=1;                                 
	        vrd = imageLoad(rgbaImage, storePos);            
	        storePos.x-=1;                                 
	        vld = imageLoad(rgbaImage, storePos);            
	                                      
		y0 = 0.299*vl.r + 0.587 *vl.g + 0.114*vl.b;
		u0 = (-0.169*vl.r - 0.331*vl.g + 0.500*vl.b) + 0.5;
		v0 = (0.500*vl.r - 0.419*vl.g - 0.081*vl.b) + 0.5;

		y1 = 0.299*vr.r + 0.587 *vr.g + 0.114*vr.b;
		u1 = (-0.169*vr.r - 0.331*vr.g + 0.500*vr.b) + 0.5;
		v1 = (0.500*vr.r - 0.419*vr.g - 0.081*vr.b) + 0.5;

		y2 = 0.299*vrd.r + 0.587 *vrd.g + 0.114*vrd.b;
		u2 = (-0.169*vrd.r - 0.331*vrd.g + 0.500*vrd.b) + 0.5;
		v2 = (0.500*vrd.r - 0.419*vrd.g - 0.081*vrd.b) + 0.5;

		y3 = 0.299*vld.r + 0.587 *vld.g + 0.114*vld.b;
		u3 = (-0.169*vld.r - 0.331*vld.g + 0.500*vld.b) + 0.5;
		v3 = (0.500*vld.r - 0.419*vld.g - 0.081*vld.b) + 0.5;
	                                                                                       
	        sumUV.x = (u0+u1+u2+u3)/4.0;                                                     
	        sumUV.y = (v0+v1+v2+v3)/4.0;                                                     

		// calculate position of NV components
	        nvPos.x = nvPos.x;                                                 
	        nvPos.y = nvPos.y/2;
	
		// update start position of NV buffer	
		nvPos.y += height;

		imageStore(yImage, nvPos, sumUV);
		sumUV.x = sumUV.y;
		nvPos.x += 1;
		imageStore(yImage, nvPos, sumUV);  
	       
	        yvec.x = y0;
	        imageStore(yImage, ypos, yvec); 
	        ypos.x = ypos.x+1;
	        yvec.x = y1;
	        imageStore(yImage, ypos, yvec);
	        ypos.y = ypos.y+1;
	        yvec.x = y2;
	        imageStore(yImage, ypos, yvec); 
	        ypos.x = ypos.x-1; 
	        yvec.x = y3;
	        imageStore(yImage, ypos, yvec);

        }
}                                   
