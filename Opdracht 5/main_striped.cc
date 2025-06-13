#include <cmath>
#include <complex>
#include <numbers>
#include <ostream>
#include <omp.h>
#include <iostream>
#include <mpi.h>

#include "consts.h"
#include "frame.h"
#include "animation.h"

#define cimg_display 0        // No window plz
#include "CImg.h"

using std::cout, std::endl;
using namespace std::literals::complex_literals;

// Colour based on ratio between number of iterations and MAX_ITER
inline constexpr pixel COLOURISE(double iter) { 
  iter = fmod(4 - iter * 5 / MAX_ITER, 6);
  byte x = static_cast<byte>(255 * (1 - std::abs(fmod(iter, 2) - 1)));
  byte r, g, b;

  if      (             iter < 1) { r = 255; g =   x; b =   0; }
  else if (iter >= 1 && iter < 2) { r =   x; g = 255; b =   0; }
  else if (iter >= 2 && iter < 3) { r =   0; g = 255; b =   x; }
  else if (iter >= 3 && iter < 4) { r =   0; g =   x; b = 255; }
  else if (iter >= 4 && iter < 5) { r =   x; g =   0; b = 255; }
  else                            { r = 255; g =   0; b =   x; }
  return { r, g, b };
}

frame renderFrame(unsigned int t) {
  // TODO - render frame t and store in frames[t-offset]
  frame f = frame();
  double a = 2 * std::numbers::pi * t / CYCLE_FRAMES;
  double r = 0.7885;
  std::complex<double> c = r * cos(a) + static_cast<std::complex<double>>(1i) * r * sin(a);
  double x_y_range = 2;

  // Set colour for every pixel
  #pragma omp parallel for collapse(2) num_threads(NUM_THREADS)
  for (int x = 0; x < WIDTH; x++)
  {
    for (int y = 0; y < HEIGHT; y++)
    {
      std::complex<double> z = 2 * x_y_range * std::complex(static_cast<double>(x)/WIDTH, static_cast<double>(y)/HEIGHT) - std::complex(x_y_range*3/4, x_y_range);
      double scale = 1.5 - 1.45 * log(1 + 9.0 * t / FRAMES) / log(10);
      z *= scale;

      double iteration = 0;

      while (std::norm(z) < x_y_range && iteration < MAX_ITER)
      {
        z = std::pow(z, 2) + c;
        iteration += 1;
      }

      if (iteration == MAX_ITER)
      {
        f.set_colour(x, y, pixel{0, 0, 0});
      }
      else
      {
        f.set_colour(x, y, COLOURISE(iteration));
      }
    }
  }

  return f;
}

int main (int argc, char *argv[]) {
  int id, numProcesses = -1;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

  // Needed to send frames over MPI
  MPI_Datatype mpi_img;
  MPI_Type_contiguous(FRAME_SIZE, MPI_BYTE, &mpi_img);
  MPI_Type_commit(&mpi_img);

  frame next;
  int nWorkers = numProcesses -1;

  if (id > 0)
  { // RENDER NODES
    int workerId = id -1;
    
    for (unsigned int f = workerId; f < FRAMES; f+= nWorkers) {
      frame next = renderFrame(f);
      
      MPI_Send(&next, 1, mpi_img, 0, 0, MPI_COMM_WORLD);
    }
  }
  else
  { // ROOT NODE
    double startTime, totalTime, memoryTotal, renderingTotal, savingTotal = 0.0;
    std::vector<unsigned int> framenr(nWorkers);
    for(int i = 0; i < nWorkers; i++)
    {
      framenr[i] = i;
    }

    startTime = MPI_Wtime();
    
    animation frames(FRAMES);
    memoryTotal = MPI_Wtime() - startTime;

    int iter, proc = 0; // iterator and process number
    while(iter < FRAMES)
    {
      MPI_Recv(&next, 1, mpi_img, proc + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);    // Get the frame, adding 1 to not include 
      cout << "(" << iter << ")" << " Receiving frame nr " << framenr[proc] << endl;
      frames[(int)framenr[proc]] = next;
      iter++;
      framenr[proc] += nWorkers; // update frame number
      proc = (proc + 1) % nWorkers; // cycle processes
    }
    
    renderingTotal = MPI_Wtime() - startTime - memoryTotal;

    cimg_library::CImg<byte> img(WIDTH,HEIGHT,FRAMES,3);
    cimg_forXYZ(img, x, y, z) { 
      img(x,y,z,RED) = (frames)[z].get_channel(x,y,RED);
      img(x,y,z,GREEN) = (frames)[z].get_channel(x,y,GREEN);
      img(x,y,z,BLUE) = (frames)[z].get_channel(x,y,BLUE);
    }

    std::string filename = std::string("animation.avi");
    img.save_video(filename.c_str());
    savingTotal = MPI_Wtime() - startTime - memoryTotal - renderingTotal;


    totalTime = MPI_Wtime() - startTime;
    cout << "Memory time taken \t" << memoryTotal << "s" << endl;
    cout << "Redering time taken \t" << renderingTotal << "s" << endl;
    cout << "Saving time taken \t" << savingTotal << "s" << endl;
    cout << "Total time taken \t" << totalTime << "s" << endl;
  }

  // Also needed to send frames over MPI
  MPI_Type_free(&mpi_img);
  MPI_Finalize();
}
