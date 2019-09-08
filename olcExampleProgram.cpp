#define OLC_PGE_APPLICATION
#define _USE_MATH_DEFINES
#include "olcPixelGameEngine.h"

//change the below to point to your irrKlang.h file
#include "..\\irrKlang-64bit-1.6.0\include\irrKlang.h"

#include <math.h>

//change the below to point to your irrKlang.lib
#pragma comment(lib, "C:\\Users\\James\\source\\repos\\irrKlang-64bit-1.6.0\\lib\\Winx64-visualStudio\\irrKlang.lib")

#pragma warning(disable : 4996)

using namespace irrklang;


#define dist(a,b,c,d) sqrt(double((a -c) * (a-c) + (b-d) * (b-d)))


INT32 dx = 0, dy, dv;
int r, g, b;

float sec = 0.0;

int cx = 0;
int cy = 0;

int res1 = 0, res2 = 0, res3 = 0, resfinal = 0;

float frequency = 0.08;
double timeelapsed = 0;

//precomputed tables
int precomputed_r[256];
int precomputed_g[256];
int precomputed_b[256];


//inline functions to return values from the precomputed sin tables
inline int p_red(int x) { return precomputed_r[x & 255]; }
inline int p_greed(int x) { return precomputed_g[x & 255]; }
inline int p_blue(int x) { return precomputed_b[x & 255]; }




class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Old School Plasma Effect";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here





		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		//update the timer - this is a dirty way of doing it!
		sec = sec + 0.1;


		// called once per frame

		//for loop to do the whole screen one pixel at a time (SLOOOOOW)
		for (int x = 0; x < ScreenWidth(); x++)
		{
			for (int y = 0; y < ScreenHeight(); y++)
			{



				//compute the first sin effect values
				res1 = (sin(0.04 *
					((x * sin(sec / 4)) +
					(y * cos(sec / 6)) + sec)))
					* 127 + 128;

				//compute the second sin effect values
				res2 = (sin(x * 0.045 + sec / 2))
					* 127 + 128;


				//compute the third sin effect values
				cx = x + ((ScreenWidth() / 2) * sin(sec / 10)) - (ScreenWidth() / 2);
				cy = y + (128 * cos(sec / 6)) - 128;
				res3 = (sin(sqrt(0.01 * ((cx * cx) + (cy * cy) + 1)) + sec / 2))
					* 127 + 128;

			//add the sin effect values together - we are expecting all results to be in the 0-255 range so divide by the number of effects to normalise
			resfinal = (res1 + res2 + res3 ) /3;

				

			

			/*r = (sin(resfinal * 0.015 * M_PI)) * 127 + 128;
			g = (cos(resfinal * 0.015 * M_PI)) * 127 + 128;
			b = (sin(resfinal * M_1_PI)) * 127 + 128;*/

			//r = (sin(0.015 * resfinal * M_PI)) * 127 + 128;
			r = p_red(resfinal);
			g = p_greed(resfinal);
			b = p_blue(resfinal);


			//if (r > 255) r = 255;
			//if (r < 0) r = 0;

			Draw(x, y, olc::Pixel(r, g, b));
		
			}
		}
		
		
		
		return true;



	}
};


int main()
{

	//precompute the sin tables for the colours of the plasma
	struct table_filler {
		table_filler() {
			for (int i = 0; i < 256; i++) {
				precomputed_r[i] = (sin(0.015 * i * M_PI)) * 127 + 128;
				precomputed_g[i] = (sin(0.015 * i * M_PI + 2 * M_PI / 3)) * 127 + 128;
				precomputed_b[i] = (sin(0.015 * i * M_PI + 4 * M_PI / 3)) * 127 + 128;
			}
		}
	} table_filler_instance;
	
	//call the IrrKlang engine setup
	ISoundEngine* engine = createIrrKlangDevice();
	if (!engine) return 0;

	//playback the S3M file
	engine->play2D("2ND_PM.S3M", true);
	
	//create the demo interface with OLC Pixel engine
	Example demo;
	if (demo.Construct(256, 256, 4, 4))
		demo.Start();


	// drop the IrrKlang engine
	engine->drop();
	return 0;
}
