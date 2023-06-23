#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SCR 30

char chooseLuminace(double light);

uint8_t asset(int x, int y, double z);

void randerFrame(double dObj, double A, double B) {
  double xL = 1;
  double yL = 0;
  double zL = 0;
  double exposeFactor = 0.95;

  double zBuffer[SCR][SCR];
  memset(zBuffer, 0x0, sizeof(double) * SCR * SCR);
  char frame[SCR][SCR];
  memset(frame, ' ', sizeof(char) * SCR * SCR);
  double x, y, z;

  const float r1 = 1; // bisic circle radius
  const float r2 = 2;
  double the = 0; // bisic circle
  double phi = 0; // rotate circle forming donut

  const double theSpacing = M_PI / 60;
  const double phiSpacing = M_PI / 60;
  double dScr;

  // no exceeding
  dScr = SCR * dObj * 3 / (8 * (r1 + r2));

  double sinB = sin(B), cosB = cos(B);
  double sinA = sin(A), cosA = cos(A);
  for (phi = 0; phi < M_PI * 2; phi += phiSpacing) {
    double sinphi = sin(phi), cosphi = cos(phi);
    for (the = 0; the < M_PI * 2; the += theSpacing) {
      double sinthe = sin(the), costhe = cos(the);

      double circlex = r2 + r1 * costhe;
      double circley = r1 * sinthe;
      /* x = circlex * cosphi; */
      /* y = circley; */
      /* z = circlex * sinphi; */
      /* x = circlex * cosphi; */
      /* y =(circlex * sinphi * sinA + circley * cosA); */
      /* z = circlex * sinphi * cosA - circley * sinA; */
      x = circlex * cosphi * sinB +
          cosB * (circlex * sinphi * sinA + circley * cosA);
      y = circlex * cosphi * cosB -
          sinB * (circlex * sinphi * sinA + circley * cosA);
      z = circlex * sinphi * cosA - circley * sinA;

      int x_ = (int)((x * dScr) / (dObj + z));
      int y_ = (int)((y * dScr) / (dObj + z));

      if (!asset(x_, y_, z)) {
        continue;
      }

      double xN = costhe * cosphi * sinB +
                  cosB * (sinthe * cosA - costhe * sinphi * sinA);
      double yN = costhe * cosphi * cosB -
                  sinB * (sinthe * cosA - costhe * sinphi * sinA);
      double zN = sinthe * sinA + costhe * sinphi * cosA;

      double light = xN * xL + yN * yL + zN * zL;
      light /= sqrt(xL * xL + yL * yL + zL * zL);
      light *= exposeFactor;

      int mapX = x_ + SCR / 2;
      int mapY = y_ + SCR / 2;
      if (zBuffer[mapY][mapX] > 1 / (z + dObj)) {
        continue;
      }
      zBuffer[mapY][mapX] = 1 / (z + dObj);
      frame[mapY][mapX] = chooseLuminace(light);
    }
  }

  for (int i = 0; i < SCR; i++) {
    for (int j = 0; j < SCR; j++) {
      printf("%c%c", frame[i][j], frame[i][j]);
    }
    printf("\n");
  }
}

void msSleep(long msec) {
  struct timespec ts;
  int res;
  if (msec < 0) {
    return;
  }

  ts.tv_sec = msec / 1000;
  ts.tv_nsec = (msec % 1000) * 1000000;

  nanosleep(&ts, &ts);
}

int main() {
  double i = 0, j = 0;
  while (1) {
    randerFrame(5, i, j);
    i += M_PI / 80;
    j += M_PI / 170;
    msSleep(10);
  }
}

uint8_t asset(int x, int y, double z) {
  x += SCR / 2;
  if (x > SCR - 1 || x < 0) {
    return 0;
  }
  y += SCR / 2;
  if (y > SCR - 1 || y < 0) {
    return 0;
  }
  return 1;
}

char chooseLuminace(double light) {
  const static char *const luminace = ".,-~:;=!*#$@";

  if (light <= 0) {
    return '.';
  }

  light *= 12;
  int grade = (int)light;
  if (grade >= 11) {
    grade = 11;
  }
  return luminace[grade];
}
