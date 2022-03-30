We have two pictures - AskhatCat.bmp and table.bmp. We want to put the cat on the tennis table. The following formula was used in the work:

**colour_res = (colour_front * alpha + color_back * (1 - alpha)) / 256**, 

where alpha is the transparency of the current pixel. This formula calculates a new value for all pixels for red, blue, and green, respectively.

Writing the program that makes alpha-blending is not a very big task. I decided to the measurement the time of work of the program. The results weren't very satisfying, so I started thinking about how I could improve them.

In the original version, iterating over pixel coordinates required two nested loops. It seems that iterating over the elements can be improved a little, knowing that in addition to the int, double data types, there are extended types - SSE and AVX. I decided to take the coordinates in packs of 4 and put them in variables like __m128i. 

The screenshot shows the results of accelerating the program. In the upper window is the slow version, in the lower window the version using SSE. The first launch was made with drawing a happy cat lying on the table, the second - without drawing (to look at the time spent without taking this process into account; i.e. new coordinates are calculated, but the picture itself is not drawn).

Let's look at the table that summarizes the results:

| Program | With drawing (ms) | Without drawing (ms) |
|----------------|--------|----------------|
| Without acceleration | 414 | 406 | 
| With acceleration | 300 | 295 |

So, we can see that the program has accelerated by about 28% (without rendering, the percentage is 27.5, which is approximately equal to that obtained with rendering). The measurements were carried out with the optimization -O1.

By the way, the result of overlaying images looks very nice regardless of the presence or absence of accelerations:

<img src="https://github.com/x-ENIAC/MIPT_projects_2_sem/blob/master/Cat%20and%20table/picture.png" alt="drawing" width="600"/>
