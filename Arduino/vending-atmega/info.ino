/*
 * Connection on back of tray (when looking at it and seeing motors
 * 
 * 1 |  4  |  7  |  10  |  13  |  16  |
 * 2 |  5  |  8  |  11  |  14  |  17  |
 * 3 -  6  -  9  -  12  -  15  -  18  -
 * 
 *  1-10 Ground pin from motors
 * 
 *  16 Limit Swicth - Goes HIGH after full rotation (IF ANY IN THE ENTIRE MACHINE ARE NOT OPEN THEN THE MACHINE WILL NOT WORK) 
 *  17 Tray Sense Line
 *  18 24v in - diode to 17
 */
