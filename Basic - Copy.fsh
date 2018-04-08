#version 130


uniform float alpha;
uniform sampler2D tex_real, tex_surf, tex_DI_Dark, tex_DI_Bright, tex_SI, tex_env;
uniform float dist;
//varying float F;
uniform float width, height;

//const float half_col = 3, half_row = 3;
uniform float filter_size;
uniform float amb_strength;
uniform float LOD;

//varying float filter_sum;
uniform vec3 light_dir;
uniform bool toggle_Mirror;
uniform int toggle_ShaAmbCos;
uniform float SM_Quality;

//function to calculate ambient occlusion effect of image
void AmbientShadow(in vec4 center, out float Amb)
{
    float i,j;
    float max, min;
    //    float sum = float((half_row*(half_row+1)+half_row+1)*(2*half_col+1)+(half_col*(half_col+1)+half_col+1)*(2*half_row+1));
    float diff = 0.0;
    float step = filter_size/1.0;
    float    filter_sum = 0.001;
     if(step>0.01)
    {
     // if(texture2D(tex_surf, gl_TexCoord[0].st).b!=0)
       //if(texture2D(tex_surf, gl_TexCoord[0].st).a=1)
            for(i=-(filter_size);i<(filter_size)+step/2.0;i+=step)
                for(j=-(filter_size);j<(filter_size)+step/2.0;j+=step)
                {
                    vec4 neighbor = texture2D(tex_surf, vec2(gl_TexCoord[0].s+i/width, gl_TexCoord[0].t+j/height));
                    if((i>step/2.0||i<-step/2.0)&&(j>step/2.0||j<-step/2.0))
                    {
                        //            diff += (neighbor - center)*vec2(sign(i),sign(j))*(half_row-abs(i)+half_col-abs(j))/sum;
                        diff = diff + dot(neighbor.rg - center.rg,vec2(i,j))/(abs(i)+abs(j))*neighbor.b*neighbor.a;
                        filter_sum = filter_sum+2*neighbor.b*neighbor.a;
                    }
                }
        //                        vec2 neighbor = texture2D(tex_surf, vec2(gl_TexCoord[0].s+i/width, gl_TexCoord[0].t+j/height)).rg;
        //                                        diff += dot(neighbor - center,vec2(i,j));

    }
   diff= diff*center.a;
    //filter_sum = filter_sum*amb_strength;
    Amb = (diff+filter_sum*amb_strength)/((0.1+1.4*amb_strength)*filter_sum);
    Amb=Amb+amb_strength;
   if (Amb>1) Amb=1; if  (Amb<0) Amb=0;

    //    return texture2D(tex_surf, gl_TexCoord[0].st+0.1);
}




//function to calculate shadow effect of image
void Shadow1(in vec2 center, out float pure_cos, out float Sha, out float Spe)
{  
    
    vec2 light_dir_xy = normalize(light_dir.rg);
    vec3 light_dir_n = normalize(light_dir);
    //float SM_Quality = 0.125;
    vec4 neighbor = SM_Quality*(2*texture2D(tex_surf, gl_TexCoord[0].st )-1.0); 
    float   temp    = 1-neighbor.r*neighbor.r-neighbor.g*neighbor.g;  if(temp<0.) temp=0.;
    vec3 center_normal   = normalize(vec3(neighbor.r, neighbor.g, sqrt(temp)));

    vec3 r = 2.0* dot( vec3(0, 0, 1), center_normal ) - vec3(0, 0, -1);   
    Spe = pow( dot(vec3(-light_dir_n.r, -light_dir_n.g, light_dir_n.b), r), 5.0);


    pure_cos = dot(center_normal, -light_dir_n);

    float i;
    float diff = 0.0;
    float step = 1*filter_size/10.0;
    float integral = 0.0;
    int count = 0;
    float shadow_amount=1.0;
    if(step>0.005)
    {
        {
            for(i=0;i<(1*filter_size)+step/2;i+=step)
            {
                vec4 neighbor = SM_Quality*(2.0*texture2D(tex_surf, gl_TexCoord[0].st - i*light_dir_xy/vec2(width, height))-1.0);
               
                temp = 1-neighbor.r*neighbor.r-neighbor.g*neighbor.g; if(temp<0.1) temp=0.1;
                neighbor.z =      sqrt(temp); 
                neighbor = normalize(neighbor);
                integral += (-light_dir_xy.x*neighbor.r-light_dir_xy.y*neighbor.g)/neighbor.z;
                count++;
            }
        }
    }
   

    Sha =( integral - light_dir_n.z )/(count); if(Sha<0) Sha=0; if(Sha>1.) Sha=1.;
}



//function to calculate the reflection and refraction effect of image
void ReflectRefract(in vec4 cood_color, out vec4 refractColor, out vec4 reflectColor)
{
    //get color(coordinates) of surface image

    vec2 cood_refl = (cood_color.st);
    vec2 cood_displace = (cood_color.st-0.49803921568627450980392156862745)*cood_color.p*dist;
    vec2 cood_refr = gl_TexCoord[0].st+cood_displace;

    refractColor =  textureLod(tex_real, cood_refr, LOD+20*length(cood_displace));
    reflectColor =  textureLod(tex_env, cood_refl+(light_dir.rg)*0.2/(0.1+abs(light_dir.b)), LOD);
}

void FrenselBlend(in vec4 cood_color, in vec4 refractColor, in vec4 reflectColor, out vec4 Ref_Fresnel)
{
    float Ratio;
    if(toggle_Mirror)
        {Ratio=1; if(cood_color.p < 1) Ratio=Ratio*cood_color.p;}
    else
    {
        float temp = abs(cood_color.s -0.5);
        float temp2 = abs(cood_color.t -0.5);
        if(temp < temp2 ) temp = temp2;
        float teta = 2.0* temp;
        float t = (0.5+0.5*dist);
        float c = 0.8*(1.0-t)+1.0*t;
        float bc= 0.6*(1.0-t)+0.7*t;
        float ac= 0.5*(1.0-t)+0.2*t;
        float b=  1.0*(1.0-t)+0.1*t;
        if(teta< ac)    {
            Ratio= b- b/ac*teta;
        }
        else
            if(teta< bc)    {
                Ratio = 0.0;
            }
            else        {
                Ratio= (teta -bc) /(c-bc);
            }
        if(Ratio>1.0)
            Ratio = 1.0;
        //    cood_color.p*2-1;
        //Ratio=Ratio/2.0;
    }
    //the reflection and refraction blended based on Fresnel
    Ref_Fresnel  = mix(refractColor, reflectColor, Ratio);
}



void main()
{
    //get the (x, y) normal vector from R and G channel
    vec4 cood_center = (texture2D(tex_surf, gl_TexCoord[0].st));

    //the mask from foreground image(specular image)
    //vec4 mask = (texture2D(tex_SI, gl_TexCoord[0].st));

    //shadow and ambient occlusion calculated based on functions
    //these are the outputs, corresponds to cos_theta, Shadow strength, Ambiend shadow strength
    float center_cos, pure_cos, Sha, Amb,  Spe;

    Shadow1(cood_center.st, pure_cos, Sha, Spe);
    AmbientShadow(cood_center, Amb);
    vec3 light_dir_n = normalize(light_dir);
    float b=1.0, a=sqrt(1-(light_dir_n.z*light_dir_n.z));
    center_cos=(pure_cos-a)/(b-a); if(center_cos<0.0) center_cos=0.0; if(center_cos>1.0) center_cos=1.0;
 //   center_cos = pure_cos;

    if(toggle_ShaAmbCos == 0)
        center_cos = 0;
    else if(toggle_ShaAmbCos == 1)
        center_cos = center_cos;
    else if(toggle_ShaAmbCos == 2)
        center_cos = Amb;
    else if(toggle_ShaAmbCos == 4)
        center_cos = Sha;
    else if(toggle_ShaAmbCos == 3)
        center_cos = center_cos*Amb;
    else if(toggle_ShaAmbCos == 5)
        center_cos = (center_cos+Sha)/2.;
    else if(toggle_ShaAmbCos == 6)
        center_cos=Sha*Amb;
    else if(toggle_ShaAmbCos == 7)
        center_cos=center_cos*Sha*Amb;

    //blend Diffusion image 1 and 2 based on shadow strength
    vec4 Shadow_blend = mix( texture2D(tex_DI_Dark, gl_TexCoord[0].st),  texture2D(tex_DI_Bright, gl_TexCoord[0].st), center_cos);
    vec4 Light_direct = mix(Shadow_blend, texture2D(tex_SI, gl_TexCoord[0].st), 0);
	//vec4 Light_direct = mix(vec4(0.0),vec4(1.0), Sha*Amb);
    //vec4 Light_direct =vec4(1.0)*center_cos;

    //calculate refraction/reflection with Fresnel
    vec4 refl_color, refr_color, Light_indirect; //reflection, refraction, and blended color
    ReflectRefract(cood_center, refr_color, refl_color);
    FrenselBlend(cood_center, refr_color, refl_color, Light_indirect);

    //blend Shadow image and reflection/refraction image half-half

    vec4 Fore_blend = mix(Light_indirect, Light_direct, alpha*Light_direct.a);

    //blend all image with foreground image
//    gl_FragColor   = Light_indirect;
//    gl_FragColor =  textureLod(tex_real, gl_TexCoord[0].st, filter_size*2);
    float Final_Mix = cood_center.a; if (cood_center.b<0.001) Final_Mix=Final_Mix*cood_center.b;
    gl_FragColor = mix(texture2D(tex_real, gl_TexCoord[0].st), Fore_blend, Final_Mix);
//    gl_FragColor = vec4(1.0)*Amb;
//    gl_FragColor = vec4(1.0)*Spe;
}
