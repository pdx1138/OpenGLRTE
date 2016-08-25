// RayTrace.cpp
/////////////////////////////////////////////////////////////////////////////////////

#include "RayTrace.h"

Camera* mainCamera = nullptr;
Scene* scene = nullptr;

Texture* textureTest = nullptr;

unsigned int nBgColor = CreateColor(33, 185, 3, 255);

Scene* GetNewScene() {
	return new Scene();
}

void CalculateLighting(Camera* pCam, Light* light, RayHitInfo &hitInfo, int recursionDepth, bool ambOcc) {
	Vector lightVector;
	if(light->type == DIR_LIGHT) {
		lightVector = -1.0f * light->direction;
	}
	else if (light->type == POINT_LIGHT) {
		lightVector = light->origin - hitInfo.point;
	}
	lightVector.Normalize();

	RayHitInfo info;

	/*if(ambOcc == true) {
		Vector camVector = pCam->position - hitInfo.point;
		float dist = camVector.Length();
		float maxLength = MAX_AMB_OCC_LENGTH;
		float z = dist / maxLength;
		if (z > 1.0f) z = 1.0f;
		if (z < 0.0f) z = 0.0f;
		z = 1.0f - z;
		Color c = Color(z, z, z, 1.0f);
		c.Clamp();
		hitInfo.pixel += ConvertColor(c);
		return;
	}*/

	#ifdef ENABLE_AMBIENT_OCCLUSION
		Color avg = Color (0.0f, 0.0f, 0.0f, 0.0f);
		if(light->type != DIR_LIGHT) {
			Vector T = CrossProduct(hitInfo.normal, Vector(0.0f, 1.0f, 0.0f, 1.0f));
			Vector B = CrossProduct(T, hitInfo.normal);

			const int W = MAX_ABM_OCC_WINDOW;
			const int H = MAX_ABM_OCC_WINDOW;

			int image_buffer[W * H];
			HemisphericalCamera hemiCam;
			hemiCam.image_plane_width = W;
			hemiCam.image_plane_height = H;
			hemiCam.orientation.forward = hitInfo.normal;
			hemiCam.orientation.right = T;
			hemiCam.orientation.up = B;
			hemiCam.position = hitInfo.point;
			RayTrace(image_buffer, W, H, &hemiCam, scene, recursionDepth, true);

			for (int n = 0; n < W*H; ++n) {
				int pixel = image_buffer[n];
				Color c = ConvertColor(pixel);
				avg = avg + c;
			}
			avg = avg / ((float)(W*H));
			avg.Clamp();
			
		} else {
			avg.r = 0.0f;
		}

		float ambientOcclusionIntensity = 1.0f - avg.r;
	#else
		float ambientOcclusionIntensity = 1.0f;
	#endif

	// Shadows
	#ifdef ENABLE_SHADOWS
		Ray rayToLight;
		int numHits = 0;

		rayToLight.origin = hitInfo.point;
		rayToLight.direction = lightVector;
		rayToLight.direction.Normalize();
		if(GetNearestHit(rayToLight, scene, info)) {
			hitInfo.pixel = ConvertColor(hitInfo.pHitObject->color * Color(0.01f, 0.01f, 0.01f, 1.0f));
			return;
		}/**/
		float shadowIntensity = 1.0f;
	#else
		#ifdef ENBALE_SOFTSHADOWS
			int numHits = 0;
			if(light->type != DIR_LIGHT) {
				Ray rayToLight;
				//int numHits = 0;

				for (int n = 0; n < NUM_SHADOW_RAYS; ++n) {
					rayToLight.origin = hitInfo.point;
				
					Point lightPosition = light->origin;
					lightPosition.x += (10.0f * ((rand()%1000)/1000.0f)) - 5.0f;
					lightPosition.y += (10.0f * ((rand()%1000)/1000.0f)) - 5.0f;
					lightPosition.z += (10.0f * ((rand()%1000)/1000.0f)) - 5.0f;
					/*lightPosition.x += (20.0f * ((rand()%1000)/1000.0f)) - 10.0f;
					lightPosition.y += (20.0f * ((rand()%1000)/1000.0f)) - 10.0f;
					lightPosition.z += (20.0f * ((rand()%1000)/1000.0f)) - 10.0f;*/
					lightPosition.w = 0.0f;										   

					rayToLight.direction = lightPosition - hitInfo.point;
					rayToLight.direction.Normalize();
					info = hitInfo;
					if(GetNearestHit(rayToLight, scene->objects, info)) {
						numHits += 1;
					}
				}
			}
			else {
				numHits = 0;
			}

			float shadowIntensity = 1.0f - (((float)numHits) / NUM_SHADOW_RAYS);
		#else
			float shadowIntensity = 1.0f;
		#endif
	#endif

	#ifdef ENABLE_REFLECTIONS
		// Reflections
		Vector reflection = Reflection(hitInfo.ray.direction, hitInfo.normal);
		Ray reflectionRay;
		Color reflectionColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
		reflectionRay.direction = reflection;
		reflectionRay.origin = hitInfo.point;
		if(RayTrace(reflectionRay, scene->objects, scene->lights[0], info, recursionDepth)) {
			reflectionColor = ConvertColor(info.pixel);
		}/**/

		reflectionColor = reflectionColor * hitInfo.pHitObject->reflectivity;
	#else
		Color reflectionColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
	#endif

		// Texture
	#ifdef ENABLE_TEXTURE_MAPPING
		int nTextureColor = 0;
		/*if(hitInfo.pHitObject->texture->id != 5000) nTextureColor = GetTexel(textureTest, hitInfo.tu, hitInfo.tv);*/
		if(hitInfo.pHitObject->texture.id != -1) nTextureColor = GetTexel(textureTest, hitInfo.tu, hitInfo.tv);
		else {
			nTextureColor = GetTexel(&hitInfo.pHitObject->texture, hitInfo.tu, hitInfo.tv);
		}
		Color textureColor = ConvertColor(nTextureColor);
	#else
		Color textureColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
	#endif
	
	Vector surfaceNormal = hitInfo.normal;
	surfaceNormal.Normalize();

	#ifdef ENABLE_SPECULAR_LIGHTING
		Vector cameraVector = mainCamera->position - hitInfo.point;
		cameraVector.Normalize();

		Vector halfwayVector = lightVector + cameraVector;
		halfwayVector.Normalize();

		float specularIntensity = DotProduct(halfwayVector, surfaceNormal);
		if(specularIntensity < 0.0f) specularIntensity = 0.0f;

		float specularPower = 60.0f;
		specularIntensity = pow(specularIntensity, specularPower);
		
		#ifdef ENABLE_TOON_SHADING
			ToonRamp(specularIntensity);
		#endif

		Color specularColor = light->intensity * specularIntensity * Color(1.0f, 1.0f, 1.0f, 1.0f);
	#else
		Color specularColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
	#endif	

	#ifdef ENABLE_FRESNEL_LIGHT
		Vector fresnelVector = mainCamera->position - hitInfo.point;
		fresnelVector.Normalize();
		Vector n = hitInfo.normal;
		n.Normalize();
		float fresnelDot = DotProduct(n, fresnelVector);
		float fresnelIntensity = 1.0f - fresnelDot;
		float fresnelPower = 2.0f;
		fresnelIntensity = (float)pow(fresnelIntensity, fresnelPower);
		ClampZeroOne(fresnelIntensity);

		//Color fresnelColor = Color(0.5f, 0.5f, 0.5f, 1.0f);
		//Color fresnelColor = Color(0.1f, 0.1f, 0.1f, 1.0f);
		//Color fresnelColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
		Color fresnelColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
		
		/*#ifdef ENABLE_TOON_SHADING
			Color fresnelColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
		#else
			Color fresnelColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
		#endif*/

	#else
		//float fresnelIntensity = 1.0f;
		//Color fresnelColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
		float fresnelIntensity = 0.0f;
		Color fresnelColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
	#endif

	#ifdef ENABLE_TOON_SHADING
		ToonRamp(fresnelIntensity);
	#endif
	Color fresnelColorResult = light->intensity * fresnelIntensity /** light->color*/ * fresnelColor;

	// DIFFUSE / AMBIENT

	float diffuseIntensity = DotProduct(lightVector, surfaceNormal);
	if(diffuseIntensity < 0.0f) diffuseIntensity = 0.0f;

	#ifdef ENABLE_TOON_SHADING
		ToonRamp(diffuseIntensity);
	#endif

	Color ambientColor =  Color(0.0f, 0.0f, 0.0f, 1.0f);
	Color diffuseColor = light->intensity * diffuseIntensity * hitInfo.pHitObject->color;

	if(light->type == DIR_LIGHT) {
		shadowIntensity = 1.0f;
		reflectionColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
		specularColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	//Color resultColor = fresnelColorResult + ambientColor + (diffuseColor * textureColor) + specularColor + reflectionColor;
	Color resultColor = ambientColor + (diffuseColor * textureColor/*/ 2 textureColor*/) + (specularColor * fresnelColorResult) + reflectionColor;
	//Color resultColor = ambientColor + (diffuseColor * textureColor) + specularColor + reflectionColor;
	
	resultColor = ambientOcclusionIntensity * resultColor;
	
	// AO WHITE SCALE
	//resultColor = ambientOcclusionIntensity * Color(1.0f, 1.0f, 1.0f, 1.0f);
	// AO GREY SCALE
	//resultColor = ambientOcclusionIntensity * Color(0.7f, 0.7f, 0.7f, 1.0f);

	resultColor = shadowIntensity * resultColor;
	resultColor.Clamp();
	hitInfo.pixel += ConvertColor(resultColor);
}

void RayTrace (RenderRegion* region, int* vidMem, int VP_WIDTH, int VP_HEIGHT, Camera* pCam, Scene* scene, int recursionDepth, bool ambOcc) {
	const float xSamples = 5;
	const float ySamples = 5;
	int n = (int)sqrt((float)(xSamples*ySamples));
	
	for(int y = region->y1 - (VP_HEIGHT / 2); y <= region->y2 - (VP_HEIGHT / 2); ++y) {
		for(int x = region->x1 - (VP_WIDTH / 2); x <= region->x2 - (VP_WIDTH / 2); ++x) {
			Color resultColor = Color(0.0f, 0.0f, 0.0f, 0.0f);

			#ifdef ENABLE_ANTI_ALIASING
			// Anti-Aliasing
			//****************************************************************************************
			for(int ysample = 0; ysample < xSamples; ++ysample) {
				for(int xsample = 0; xsample < ySamples; ++xsample) {
					RayHitInfo hitInfo;
					
					//float ppX = x + ((xsample * RandFloat()) / xSamples);
					//float ppY = y + ((ysample * RandFloat()) / ySamples);
					float ppX = x - 0.5 * (VP_WIDTH / 2) + (xsample + RandFloat()) / n;
					float ppY = y - 0.5 * (VP_HEIGHT / 2) + (ysample + RandFloat()) / n;
					Ray ray = pCam->GetRay(ppX, ppY);
					
					if(GetNearestHit(ray, scene, hitInfo) == true) {
						for(int n = 0; n < scene->lights.length; ++n) {
							CalculateLighting(pCam, scene->lights[n], hitInfo, recursionDepth, ambOcc);
						}
						resultColor += ConvertColor(hitInfo.pixel);
					} 
					else {
						resultColor += ConvertColor(nBgColor);
					}
				}
			}
			//****************************************************************************************

			/*resultColor.r /= (xSamples * ySamples);
			resultColor.g /= (xSamples * ySamples);
			resultColor.b /= (xSamples * ySamples);*/
			#else
				RayHitInfo hitInfo;
				Ray ray = pCam->GetRay(x, y);

				if(GetNearestHit(ray, scene, hitInfo) == true) {
					for(int n = 0; n < scene->lights.length; ++n) {
						CalculateLighting(pCam, scene->lights[n], hitInfo, recursionDepth, ambOcc);
					}
					resultColor += ConvertColor(hitInfo.pixel);
				} 
				else {
					resultColor += ConvertColor(nBgColor);
				}
			#endif

			PutPixel(vidMem, 
					 x + (VP_WIDTH / 2.0f), 
					 y + (VP_HEIGHT / 2.0f), 
					 VP_WIDTH, VP_HEIGHT, 
					 ConvertColor(resultColor));			
		}
	}
}

//void RayTrace (int* vidMem, int VP_WIDTH, int VP_HEIGHT, Camera* pCam, Scene* scene, int recursionDepth, bool ambOcc) {
//	for(int y = -(VP_HEIGHT / 2); y <= (VP_HEIGHT / 2); ++y) {
//		for(int x = -(VP_WIDTH / 2); x <= (VP_WIDTH / 2); ++x) {
//			Color resultColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
//
//			RayHitInfo hitInfo;
//			Ray ray = pCam->GetRay(x, y);
//
//			if(GetNearestHit(ray, scene, hitInfo) == true) {
//				if(ambOcc == true) CalculateLighting(pCam, scene->lights[0], hitInfo, recursionDepth, ambOcc);
//				else {
//					for(int n = 0; n < scene->lights.length; ++n) {
//						CalculateLighting(pCam, scene->lights[n], hitInfo, recursionDepth, ambOcc);
//					} 
//				}
//				resultColor += ConvertColor(hitInfo.pixel);
//			} 
//			else {
//				resultColor += ConvertColor(0x00000000);
//			}
//
//			PutPixel(vidMem, 
//					 x + (VP_WIDTH / 2.0f), 
//					 y + (VP_HEIGHT / 2.0f), 
//					 VP_WIDTH, VP_HEIGHT, 
//					 ConvertColor(resultColor));			
//		}
//	}
//}
//
bool RayTrace(const Ray &ray, List<Object*> &objList, Light* light, RayHitInfo &hitInfo, int recursionDepth) {
	if(recursionDepth > RAY_RECURSSION_DEPTH) return false;
	if(GetNearestHit(ray, objList, hitInfo)) {
		CalculateLighting(mainCamera, light, hitInfo, recursionDepth + 1);
		return true;
	}
	return false;
}

float RandFloat() {
	//float randNum = ((double) rand() / (RAND_MAX));
	//return ((randNum - 0) / (RAND_MAX - 0));
	return ((double) rand() / (RAND_MAX));
}

int GetTexel(Texture* texture, float tu, float tv) {
	int x = tu * texture->nWidth;
	int y = tv * texture->nHeight;
	x = x % texture->nWidth;
	y = y % texture->nHeight;
	return ((int*)texture->data)[y * texture->nWidth + x];
}

int Swizzle(int pixel) {
	// ABGR --> ARGB
	unsigned int r, g, b, a;
	ExtractColor(pixel, r, g, b, a);
	return CreateColor(b, g, r, a);
}

void Swizzle(int* video, int width, int height) {
	for(int n = 0; n < width * height; ++n) {
		video[n] = Swizzle(video[n]);
	}
}