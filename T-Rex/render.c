#include <stdint.h>
#include <pic32mx.h>
#include "game.h"


void render(void){

	/*
	 *	The display on the chipKit-board is divided into four horizontal
	 *	pages and 128 vertical columns, the following for loop sends one
	 *	hexadecimal number representing a 8-pixel high column to the display
	 *	for every page and column.
	 */

	int page, col, t, i;
	for(page = 0; page < 4; page++){
		for(col = 0; col < 128; col++){

			switch(page){

				case 0:

				/* If the dino has the same x-value as the current column we
					render the dino, depending on the y-value of the dino we
					also shift the dino to match the current vertical position  */

					if(dino.x == col){
						for(t = 0; t < dino.WIDTH; t++){

							//If bird.x = col during dino render, render both
							if(bird.x == col && bird.y == 20){
								for(i = 0; i < bird.WIDTH; i++){
									if(dino.y >= 8){
										spi_send_recv( (dinoHead[t]>>(dino.y-8)) | (dinoBody[t] << (16-dino.y)) | birdIm[i] );
									}
									else{
										spi_send_recv( dinoHead[t] << (8-dino.y) | birdIm[i]);
									}
									t++;
								}
								col += (bird.WIDTH -1);
							}
							else{
								if(dino.y >= 8){
									spi_send_recv( (dinoHead[t]>>(dino.y-8)) | (dinoBody[t] << (16-dino.y)) );
								}
								else{
									spi_send_recv( dinoHead[t] << (8-dino.y));
								}
							}

							col++;
						}
						col--;
					}

					//If we pass the birds x-value and it has the correct y-value we print it
					else if(bird.x == col && bird.y == 20){
						for(t = 0; t < bird.WIDTH; t++){
							spi_send_recv(birdIm[t]);
						}
						col += bird.WIDTH-1;
					}

					else{
						spi_send_recv(0x00);
					}
					break;

				case 1:

					if(dino.x == col){
						for(t = 0; t < dino.WIDTH; t++){

							if(bird.x == col && bird.y == 12){
								for(i = 0; i < bird.WIDTH; i++){
									spi_send_recv(birdIm[i]);
									t++;
								}
								col += bird.WIDTH-1;
							}
							else{
								if(dino.y <= 8){
									if(!ducking){
										spi_send_recv( (dinoHead[t]>>dino.y) | (dinoBody[t] << (8-dino.y)) );
									}
									else{
										spi_send_recv(0x00);
									}
								}
								else{
									spi_send_recv( dinoBody[t] >> (dino.y-8));
								}
							}
							col++;
						}
						col--;
					}


					else if(bird.x == col && bird.y == 12){
						for(t = 0; t < bird.WIDTH; t++){
							spi_send_recv(birdIm[t]);
						}
						col += bird.WIDTH-1;
					}

					else{
						spi_send_recv(0x00);
					}
					break;

				case 2:

					if(dino.x == col){

							//If dino is ducking, render ducking image
							if(ducking){
								for(t = 0; t < dino.WIDTH; t++){
									spi_send_recv(dinoDuck[t]);
								}
								col += (dino.WIDTH-1);
							}

							else{
								for(t = 0; t < dino.WIDTH; t++){

									/* If cactus.x is reached during render of dino, render cactus
									and dino. */
									if(cactus.x == col){
										for(i = 0; i < cactus.WIDTH; i++){
											spi_send_recv(dinoBody[t]>>dino.y | cactusIm[i]);
											t++;
										}
										col += (cactus.WIDTH -1);
									}
									else if(bird.x == col && bird.y == 4){
										for(i = 0; i < bird.WIDTH; i++){
											spi_send_recv(dinoBody[t]>>dino.y | birdIm[i]);
											t++;
										}
										col += (bird.WIDTH -1);
									}
									else{
										spi_send_recv(dinoBody[t]>>dino.y);
									}
									col++;
								}
								col--;
							}
					}

					else if(cactus.x == col){
						for(t = 0; t < cactus.WIDTH; t++){
							spi_send_recv(cactusIm[t]);
							if(col == 127){
								break;
							}
						}
						col += cactus.WIDTH-1;
					}

					else if(bird.x == col && bird.y == 4){
						for(t = 0; t < bird.WIDTH; t++){
							spi_send_recv(birdIm[t]);
						}
						col += bird.WIDTH-1;
					}

					else{
						spi_send_recv(0x00);
					}
					break;

				case 3:

					spi_send_recv(0x01);
					break;


			}

		}
	}
}

void renderStartScreen(void){

	//Render start screen
	int page, col, t;

	for(page = 0; page < 4; page++){
		for(col = 0; col < 128; col++){

				if(page == 0 && col == 40){
					for(t = 0; t < 35; t++){
						spi_send_recv(start[t]);
					}
					col += 34;
				}
				else if(page == 1 && col == 20){
					for(t = 0; t < 8; t++){
						spi_send_recv(dinoHead[t]);
					}
					col += 7;
				}
				else if(page == 2 && col == 20){
					for(t = 0; t < 8; t++){
						spi_send_recv(dinoBody[t]);
					}
					col += 7;
				}
				else if(page == 3){
					spi_send_recv(0x01);
				}
				else{
					spi_send_recv(0x00);
				}

		}
	}
}


void renderEndScreen(){

		//Your score
		int n, j;
		if(score < 10){
			n = 1;
		}
		else if(score < 100){
			n = 2;
		}
		else{
			n = 3;
		}

		int scores[n];
		for(j = n-1; j >= 0; j--){
			scores[j] = score % 10;
			score /= 10;
		}


		//Highscore
		if(currentHighscore < 10){
			n = 1;
		}
		else if(currentHighscore < 100){
			n = 2;
		}
		else{
			n = 3;
		}

		int highs[n];

		for(j = n-1; j >= 0; j--){
			highs[j] = currentHighscore % 10;
			currentHighscore /= 10;
		}

		int page, col, t, i;
		int nmrs[3] = {1,2,3};

		for(page = 0; page < 4; page++){
			for(col = 0; col < 128; col++){

				switch(page){

					case 0:

						if(col == 20){
							for(t = 0; t < 43; t++){
								spi_send_recv(game[t]);
							}
							col += 42;
						}
						else{
							spi_send_recv(0x00);
						}

						break;

					case 1:

						if(col == 20){

							for(t = 0; t < 46; t++){
								spi_send_recv(highscoreText[t]);
							}
							col += 45;

							for(t = 0; t < sizeof(highs)/sizeof(highs[0]); t++){
								for(i = 0; i < 4; i++){
									spi_send_recv(numbers[highs[t]*4 + i]);
								}
								spi_send_recv(0x00);
							}
							col += (sizeof(highs)/sizeof(highs[0]))*5;
						}
						else{
							spi_send_recv(0x00);
						}

						break;

					case 2:
						if(col == 20)
						{
							for(t = 0; t < 27; t++){
								spi_send_recv(scoreText[t]);
							}
							col += 26;

							for(t = 0; t < sizeof(scores)/sizeof(scores[0]); t++){
								for(i = 0; i < 4; i++){
									spi_send_recv(numbers[scores[t]*4 + i]);
								}
								spi_send_recv(0x00);
							}
							col += (sizeof(scores)/sizeof(scores[0]))*5;

						}
						else
						{
							spi_send_recv(0x00);

						}

						break;

					case 3:
						spi_send_recv(0x00);
						break;


				}

			}
		}
}
