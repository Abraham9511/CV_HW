/* Ex5_FaceMorphing
 * Recognize the edge of A4, output edge line equation, peak
 */
#include "Ex5_FaceMorphing.h"
#include <iostream>
#include <string>

using std::cin;
using std::endl;
using std::cout;
using std::string;

int main() {
    string src_filepath, target_filepath;
    src_filepath = "1.bmp";
    target_filepath = "2.bmp";
    //while (cin >> src_filepath >> target_filepath) {
        Ex5_FaceMorphing trans(src_filepath, target_filepath);
        //trans.displaySourceImage_GetDots();
        //trans.displayTargetImage_GetDots();
        trans.readin_txt();
        //trans.Delaunay_src();
        //trans.Delaunay_target();
        //trans.showSrcTarTri();
        //trans.output_txt();
        trans.Trans();
    //}
    return 0;
}
