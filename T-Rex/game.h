/* Declare display-related functions from display_functions.c */
void display();
void display_image(int x, const uint8_t *data);
void display_init(void);
uint8_t spi_send_recv(uint8_t data);
char * itoaconv( int num );


#define BUTTON1 ((PORTD >> 7) & 0x1)
#define BUTTON2 ((PORTD >> 6) & 0x1)
#define BUTTON3 ((PORTD >> 5) & 0x1)

/* Declare bitmap array containing font */
extern const uint8_t const dinoHead[8];
extern const uint8_t const dinoBody[8];
extern const uint8_t const dinoDuck[12];
extern const uint8_t const cactusIm[5];
extern const uint8_t const birdIm[5];
extern const uint8_t const start[35];
extern const uint8_t const game[43];
extern const uint8_t const numbers[39];
extern const uint8_t const highscoreText[47];
extern const uint8_t const scoreText[27];

/* Declare text buffer for display output */
extern char textbuffer[4][16];

typedef struct gameObject{
  int x;
  int y;
  int velX;
  int velY;
  int WIDTH;
  int HEIGHT;
} gameObject;

/* Game variables */
int running;
int gameOver;
int renderCount;
int r;
int score;
int currentHighscore;

struct gameObject dino;
struct gameObject cactus;
struct gameObject bird;

int jumping;
int ducking;
 /* Game functions */
void initGame();
void tick();
void render();
void renderStartScreen();
void renderEndScreen();
