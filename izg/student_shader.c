/*!
 * @file 
 * @brief This file contains implemenation of phong vertex and fragment shader.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include<math.h>
#include<assert.h>
#include <printf.h>

#include"student/student_shader.h"
#include"student/gpu.h"
#include"student/uniforms.h"
#include "vertexPuller.h"

/// \addtogroup shader_side Úkoly v shaderech
/// @{

void phong_vertexShader(
    GPUVertexShaderOutput     *const output,
    GPUVertexShaderInput const*const input ,
    GPU                        const gpu   ){
  /// \todo Naimplementujte vertex shader, který transformuje vstupní vrcholy do clip-space.<br>
  /// <b>Vstupy:</b><br>
  /// Vstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu ve world-space (vec3) a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).<br>
  /// <b>Výstupy:</b><br>
  /// Výstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu (vec3) ve world-space a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).
  /// Výstupní vrchol obsahuje pozici a normálu vrcholu proto, že chceme počítat osvětlení ve world-space ve fragment shaderu.<br>
  /// <b>Uniformy:</b><br>
  /// Vertex shader by měl pro transformaci využít uniformní proměnné obsahující view a projekční matici.
  /// View matici čtěte z uniformní proměnné "viewMatrix" a projekční matici čtěte z uniformní proměnné "projectionMatrix".
  /// Zachovejte jména uniformních proměnných a pozice vstupních a výstupních atributů.
  /// Pokud tak neučiníte, akceptační testy selžou.<br>
  /// <br>
  /// Využijte vektorové a maticové funkce.
  /// Nepředávajte si data do shaderu pomocí globálních proměnných.
  /// Pro získání dat atributů použijte příslušné funkce vs_interpret* definované v souboru program.h.
  /// Pro získání dat uniformních proměnných použijte příslušné funkce shader_interpretUniform* definované v souboru program.h.
  /// Vrchol v clip-space by měl být zapsán do proměnné gl_Position ve výstupní struktuře.<br>
  /// <b>Seznam funkcí, které jistě použijete</b>:
  ///  - gpu_getUniformsHandle()
  ///  - getUniformLocation()
  ///  - shader_interpretUniformAsMat4()
  ///  - vs_interpretInputVertexAttributeAsVec3()
  ///  - vs_interpretOutputVertexAttributeAsVec3()
  (void)output;
  (void)input;
  (void)gpu;


    Uniforms const uniHandle = gpu_getUniformsHandle(gpu);

    UniformLocation const viewML = getUniformLocation(gpu, "viewMatrix");
    UniformLocation const projectionMatrixLocation = getUniformLocation(gpu, "projectionMatrix");

    Mat4 const*const view = shader_interpretUniformAsMat4(uniHandle, viewML);
    Mat4 const*const projection = shader_interpretUniformAsMat4(uniHandle, projectionMatrixLocation);

    Mat4 mvp;
    multiply_Mat4_Mat4(&mvp,projection,view);
    Vec4 pos4;

    Vec3 const* pos = vs_interpretInputVertexAttributeAsVec3(gpu, input, 0);
    Vec3 const* out = vs_interpretOutputVertexAttributeAsVec3(gpu, output, 0);

    copy_Vec3(out,pos);
    copy_Vec3Float_To_Vec4(&pos4,pos,1.f);

    multiply_Mat4_Vec4(&output->gl_Position, &mvp, &pos4);


    pos = vs_interpretInputVertexAttributeAsVec3(gpu, input, 1);
    out = vs_interpretOutputVertexAttributeAsVec3(gpu, output, 1);
    copy_Vec3(out,pos);
}

void phong_fragmentShader(
    GPUFragmentShaderOutput     *const output,
    GPUFragmentShaderInput const*const input ,
    GPU                          const gpu   ){
  /// \todo Naimplementujte fragment shader, který počítá phongův osvětlovací model s phongovým stínováním.<br>
  /// <b>Vstup:</b><br>
  /// Vstupní fragment by měl v nultém fragment atributu obsahovat interpolovanou pozici ve world-space a v prvním
  /// fragment atributu obsahovat interpolovanou normálu ve world-space.<br>
  /// <b>Výstup:</b><br> 
  /// Barvu zapište do proměnné color ve výstupní struktuře.<br>
  /// <b>Uniformy:</b><br>
  /// Pozici kamery přečtěte z uniformní proměnné "cameraPosition" a pozici světla přečtěte z uniformní proměnné "lightPosition".
  /// Zachovejte jména uniformních proměnný.
  /// Pokud tak neučiníte, akceptační testy selžou.<br>
  /// <br>
  /// Dejte si pozor na velikost normálového vektoru, při lineární interpolaci v rasterizaci může dojít ke zkrácení.
  /// Zapište barvu do proměnné color ve výstupní struktuře.
  /// Shininess faktor nastavte na 40.f
  /// Difuzní barvu materiálu nastavte na čistou zelenou.
  /// Spekulární barvu materiálu nastavte na čistou bílou.
  /// Barvu světla nastavte na bílou.
  /// Nepoužívejte ambientní světlo.<br>
  /// <b>Seznam funkcí, které jistě využijete</b>:
  ///  - shader_interpretUniformAsVec3()
  ///  - fs_interpretInputAttributeAsVec3()
  (void)output;
  (void)input;
  (void)gpu;

  Uniforms const gpuH = gpu_getUniformsHandle(gpu);

  // vytahni interpolovanu pozici a interpolovanou norm8lu ve work-space
  Vec3 const * const Iposition = fs_interpretInputAttributeAsVec3(gpu, input, 0);
  Vec3 const * const Inormal = fs_interpretInputAttributeAsVec3(gpu, input, 1);

  UniformLocation const camH = getUniformLocation(gpu,"cameraPosition");
  UniformLocation const ligH = getUniformLocation(gpu,"lightPosition");

  // vytahnuti uniformu
  Vec3 const*const camera = shader_interpretUniformAsVec3(gpuH,camH);
  Vec3 const*const light = shader_interpretUniformAsVec3(gpuH,ligH);

  Vec3 const lightColor =  { 1.0f, 1.0f, 1.0f };  // RGB
  Vec3 const modelColor = { 0.0f, 1.0f, 0.0f };  // G
  /*
      "     vec3 dM = vec3(0.f,1.f,0.f);  \n" // uk3.2
      "     vec3 sM = vec3(1.f,1.f,1.f);  \n" // difuzní/spekulární barvy materiálu
      "     vec3 dL = vec3(1.f,1.f,1.f);  \n"
      "     vec3 sL = vec3(1.f,1.f,1.f);  \n" // jsou difuzní/spekulární barvy světla

      "     vec3 N = normalize(n);\n" // normala fragmentu
      "     vec3 L = normalize(lightPos - fraqPosition);\n" // normalizovaný vektor z pozice fragmentu směrem ke světlu

      "     float dF = clamp(dot(N,L),0.f,1.f);\n" // skalarni soucin a orez

      "     vec3 V = normalize(cameraPos - fraqPosition);\n" // normalizovany vektor
      "     vec3 R =  reflect(-V,N);\n" // odrazeny vektor podle normaly

      "     float sF = pow((clamp(dot(R,L),0.f,1.f)),40);\n" //shines factor

      "     vec3 phong = dF*dM*dL + sF*sM*sL;\n" // vypocet barvy

      "     fColor = vec4(phong, 1.f);\n"
*/
}

/// @}
