/* Ex5_FalceMorphing
 * transform a face into another face
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

    cout << "Choose Get dots from txt file(0) or by yourself(1)" << endl;
    int tips;
    cin >> tips;

    /* When you have generated triangle and write it into txt file
     * just input 0
     */
    if (tips == 0) {
        Ex5_FaceMorphing trans(src_filepath, target_filepath);
        trans.readin_txt();
        trans.showSrcTarTri();
        trans.Trans();
    } else if (tips == 1) {
        Ex5_FaceMorphing trans(src_filepath, target_filepath);
        trans.displaySourceImage_GetDots();
        trans.displayTargetImage_GetDots();
        trans.Delaunay_src();
        trans.Delaunay_target();
        trans.output_txt();
        trans.Trans();
    } else {
        cout << "Just input 0 or 1" << endl;
        exit(-1);
    }

    return 0;
}
