#include "Ex5_FaceMorphing.h"
#include "CImg.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>
#undef Success
#include <eigen3/Eigen/Dense>

using namespace cimg_library;
using namespace Eigen;
using std::cin;
using std::endl;
using std::cout;
using std::abs;
using std::sort;
using std::string;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::istringstream;

Ex5_FaceMorphing::Ex5_FaceMorphing(const string& src_filepath, const string& target_filepath) {
    src.load_bmp(src_filepath.c_str());
    target.load_bmp(target_filepath.c_str());

    filenameSrc = src_filepath;
    filenameTar = target_filepath;

    src_w = src.width();
    src_h = src.height();
    target_w = target.width();
    target_h = target.height();

}

void Ex5_FaceMorphing::buildDelaunayEx(const vector<Dot>& vecDot, vector<Triangle>& vecTriangle) {

    int size = vecDot.size();

    if (size < 3)
        return;

    CImg<double> srcTri(src);
    CImgDisplay TSrcdisp(srcTri);

    int count = 0;

    for (int i = 0; i < size-2; ++i) {
        for (int j = i+1; j < size-1; ++j) {
            for (int k = j+1; k < size; ++k) {

                Dot a = vecDot[i];
                Dot b = vecDot[j];
                Dot c = vecDot[k];
                Triangle tri(a, b, c);

                Dot center = getTriangleCircle(tri);

                if (center.value == -1)
                    continue;

                bool isFind = true;

                for(int m = 0; m < size; ++m) {
                    Dot p = vecDot[m];

                    if (p == a || p == b || p == c) {
                        continue;
                    }

                    if (pointInCircle(p, center)) {
                        isFind = false;
                        break;
                    }
                }
                if (isFind == true) {
                    //srcTri.draw_line(center.x, center.y, a.x, a.y,DOT_COLOR);
                    //TSrcdisp.display(srcTri);
                    //TSrcdisp.wait();
                    vecTriangle.push_back(Triangle(a,b,c));
                }
            }
        }
    }
}

void Ex5_FaceMorphing::displaySourceImage_GetDots() {

    vecDotSrc.push_back(Dot(0,0,0));
    vecDotSrc.push_back(Dot(0,src_h-1,1));
    vecDotSrc.push_back(Dot(src_w-1,src_h-1,2));
    vecDotSrc.push_back(Dot(src_w-1,0,3));

    CImg<double> tSrc(src);
    CImgDisplay imgdisp(tSrc, "src image");

    while(!imgdisp.is_closed()) {
        imgdisp.wait();
        if (imgdisp.button() & 1 && imgdisp.mouse_y() >= 0) {
            Dot click = Dot(imgdisp.mouse_x(), imgdisp.mouse_y(), vecDotSrc.size());
            tSrc.draw_circle(click.x, click.y, DOT_RADIUS, DOT_COLOR);
            tSrc.display(imgdisp);
            vecDotSrc.push_back(click);
        }
    }
}

void Ex5_FaceMorphing::displayTargetImage_GetDots() {

    vecDotTar.push_back(Dot(0,0,0));
    vecDotTar.push_back(Dot(0,target_h-1,1));
    vecDotTar.push_back(Dot(target_w-1,target_h-1,2));
    vecDotTar.push_back(Dot(target_w-1,0,3));

    CImg<double> tTarget(target);
    CImgDisplay imgdisp(tTarget, "target image");
    while(!imgdisp.is_closed()) {
        imgdisp.wait();
        if (imgdisp.button() & 1 && imgdisp.mouse_y() >= 0) {
            Dot click(imgdisp.mouse_x(), imgdisp.mouse_y(), vecDotTar.size());
            tTarget.draw_circle(click.x, click.y, DOT_RADIUS, DOT_COLOR);
            tTarget.display(imgdisp);
            vecDotTar.push_back(click);
        }
    }
}

void Ex5_FaceMorphing::Delaunay_src() {
    buildDelaunayEx(vecDotSrc, vecTriSrc);
}

void Ex5_FaceMorphing::Delaunay_target() {
    for (int i = 0; i < vecTriSrc.size(); ++i) {
        Dot a = vecDotTar[vecTriSrc[i].A.value];
        Dot b = vecDotTar[vecTriSrc[i].B.value];
        Dot c = vecDotTar[vecTriSrc[i].C.value];
        vecTriTar.push_back(Triangle(a, b, c));
    }
}

void Ex5_FaceMorphing::showSrcTarTri() {
    CImg<double> srcTri(src);
    CImg<double> tarTri(target);
    CImgDisplay TSrcdisp(srcTri);
    CImgDisplay TTardisp(tarTri);

    if (vecTriSrc.size() != 28) {
        cout << "vecTriSrc: " << vecTriSrc.size() << endl;
        cout << "vecTriTar: " << vecTriTar.size() << endl;
       // exit(-1);
    }

    int i = 0;

    while(!TSrcdisp.is_closed()) {
        TSrcdisp.wait();
        if ((TSrcdisp.button() & 1) && i < vecTriSrc.size()) {
            srcTri.draw_line(vecTriSrc[i].A.x, vecTriSrc[i].A.y, vecTriSrc[i].B.x, vecTriSrc[i].B.y, LINE_COLOR);
            srcTri.draw_line(vecTriSrc[i].B.x, vecTriSrc[i].B.y, vecTriSrc[i].C.x, vecTriSrc[i].C.y, LINE_COLOR);
            srcTri.draw_line(vecTriSrc[i].A.x, vecTriSrc[i].A.y, vecTriSrc[i].C.x, vecTriSrc[i].C.y, LINE_COLOR);
            srcTri.display(TSrcdisp);

            tarTri.draw_line(vecTriTar[i].A.x, vecTriTar[i].A.y, vecTriTar[i].B.x, vecTriTar[i].B.y, LINE_COLOR);
            tarTri.draw_line(vecTriTar[i].B.x, vecTriTar[i].B.y, vecTriTar[i].C.x, vecTriTar[i].C.y, LINE_COLOR);
            tarTri.draw_line(vecTriTar[i].A.x, vecTriTar[i].A.y, vecTriTar[i].C.x, vecTriTar[i].C.y, LINE_COLOR);
            tarTri.display(TTardisp);
            ++i;
        }
    }
}

bool Ex5_FaceMorphing::readin_txt() {
    string fSrc = filenameSrc.substr(0, filenameSrc.find_last_of(".")) +".txt";
    string fTar = filenameTar.substr(0, filenameTar.find_last_of(".")) +".txt";

    readin(fSrc, vecDotSrc, vecTriSrc);
    readin(fTar, vecDotTar, vecTriTar);
    return true;
}

bool Ex5_FaceMorphing::readin(const string& filename, vector<Dot>& vecDot, vector<Triangle>& vecTri) {

    ifstream inFile(filename.c_str());
    string line;
    bool flag = true;
    while(getline(inFile, line)) {
        if (line == "[Points]") {
            continue;
        }
        if (line == "[Triangles]") {
            flag = false;
            continue;
        }
        if (flag == true) {
            istringstream istr(line);
            double x,y;
            int value;
            istr >> x >> y >> value;
            vecDot.push_back(Dot(x,y,value));
        }
        if (flag == false) {
            istringstream istr(line);
            int aValue, bValue, cValue;
            istr >> aValue >> bValue >> cValue;
            Dot a = (vecDot[aValue]);
            Dot b = (vecDot[bValue]);
            Dot c = (vecDot[cValue]);
            cout << aValue << " " << bValue << " " << cValue << endl;
            vecTri.push_back(Triangle(a,b,c));
        }
    }

    cout << "Dots" << endl;
/*
    for (auto a:vecDot) {
        cout << a.x << " " << a.y << " " << a.value << endl;
    }
    */
/*
    for (auto a : vecTri) {
        cout << a.A.x << " " << a.A.y << " " << a.A.value << endl;
        cout << a.B.x << " " << a.B.y << " " << a.B.value << endl;
        cout << a.C.x << " " << a.C.y << " " << a.C.value << endl;
    }
    */
    return true;
}

bool Ex5_FaceMorphing::output_txt() {
    string fSrc = filenameSrc.substr(0, filenameSrc.find_last_of(".")) +".txt";
    string fTar = filenameTar.substr(0, filenameTar.find_last_of(".")) +".txt";

    ofstream outputSrc(fSrc.c_str(), ios::out);
    ofstream outputTar(fTar.c_str(), ios::out);

    //if (vecTriSrc.size() != 28 || vecTriTar.size() != 28)
    //    return false;

    outputSrc << "[Points]" << endl;
    outputTar << "[Points]" << endl;

    for (int i = 0; i < vecDotSrc.size(); ++i) {
        outputSrc << vecDotSrc[i].x << " " << vecDotSrc[i].y << endl;
        outputTar << vecDotTar[i].x << " " << vecDotTar[i].y << endl;
    }

    outputSrc << "[Triangles]" << endl;
    outputTar << "[Triangles]" << endl;

    for (int i = 0; i < vecTriSrc.size(); ++i) {
        outputSrc << vecTriSrc[i].A.value << " " << vecTriSrc[i].B.value << \
            " " <<vecTriSrc[i].C.value << endl;
        outputTar << vecTriTar[i].A.value << " " << vecTriTar[i].B.value << \
            " " << vecTriTar[i].C.value << endl;
    }
    outputSrc.close();
    outputTar.close();
    return true;
}

Triangle Ex5_FaceMorphing::middleTriangle(const Triangle& a, const Triangle& b, const double& rate) {
    double ax = rate*(a.A.x) + (1-rate)*(b.A.x);
    double ay = rate*(a.A.y) + (1-rate)*(b.A.y);
    double bx = rate*(a.B.x) + (1-rate)*(b.B.x);
    double by = rate*(a.B.y) + (1-rate)*(b.B.y);
    double cx = rate*(a.C.x) + (1-rate)*(b.C.x);
    double cy = rate*(a.C.y) + (1-rate)*(b.C.y);
    return Triangle(Dot(ax,ay), Dot(bx, by), Dot(cx, cy));
}

Matrix3d Ex5_FaceMorphing::TriangleToTriangle(const double& u0, const double& v0, const double& u1, \
            const double& v1, const double& u2, const double& v2 ,\
            const double& x0, const double& y0, const double& x1, const double& y1, \
            const double& x2, const double& y2) {

    int detA;
    detA = u0*v1 + u1*v2 + u2*v0 - u2*v1 -u0*v2 - u1*v0;

    int A11, A12, A13, A21, A22, A23, A31, A32, A33;
    A11 = v1-v2;
    A21 = -(v0-v2);
    A31 = v0-v1;
    A12 = -(u1-u2);
    A22 = u0-u2;
    A32 = -(u0-u1);
    A13 = u1*v2 - u2*v1;
    A23 = -(u0*v2-u2*v0);
    A33 = u0*v1 - u1*v0;

    Matrix3d result;
    result(0,0) = (double)(x0*A11+x1*A21+x2*A31)/detA;
    result(1,0) = (double)(y0*A11+y1*A21+y2*A31)/detA;
    result(2,1) = (double)(A11+A21+A31)/detA;
    result(0,1) = (double)(x0*A12+x1*A22+x2*A32)/detA;
    result(1,1) = (double)(y0*A12 + y1*A22 + y2*A32)/detA;
    result(2,1) = (double)(A12 + A22 + A32)/detA;
    result(0,2) = (double)(x0*A13 + x1*A23 + x2*A33)/detA;
    result(1,2) = (double)(y0*A13 + y1*A23 + y2*A33)/detA;
    result(2,2) = (double)(A13 + A23 + A33)/detA;
    return result;
}

Matrix3d Ex5_FaceMorphing::wTriangleToTriangle(const Triangle& a,const Triangle& b) {
    return TriangleToTriangle(a.A.x, a.A.y, a.B.x, a.B.y, \
            a.C.x, a.C.y, b.A.x, b.A.y, b.B.x, b.B.y, \
            b.C.x, b.C.y);
}

void Ex5_FaceMorphing::Trans() {

    CImg<double> temp(src);
    CImgDisplay Ttemp(temp,"transform");

    temp.display(Ttemp);

    for (int k = 1; k <= RATE; ++k) {
        cout << k << endl;
        MTranstion(k);
        vector<Matrix3d*> vecSrcTar, vecTarSrc;
        for (int i = 0; i < vecTriSrc.size(); ++i) {
            Matrix3d HSRCM = wTriangleToTriangle(vecTriSrc[i], vecTriM[i]);
            vecSrcTar.push_back(new Matrix3d(HSRCM));
            Matrix3d HTARM = wTriangleToTriangle(vecTriTar[i], vecTriM[i]);
            vecTarSrc.push_back(new Matrix3d(HTARM));
        }

        CImg<double> targetSrc(src);
        CImg<double> targetTar(target);

        cimg_forXY(targetSrc,x,y) {
            bool isFind = false;
            for (int i = 0; i < vecTriTar.size(); ++i) {
                if (pointInTriangle(vecTriTar[i], Dot(x,y))) {
                    double tx = x*(*vecTarSrc[i])(0,0) + y*(*vecTarSrc[i])(0,1) + (*vecTarSrc[i])(0,2);
                    double ty = x*(*vecTarSrc[i])(1,0) + y*(*vecTarSrc[i])(1,1) + (*vecTarSrc[i])(1,2);

                    if (tx >= 0 && tx < src_w && ty >= 0 && ty < src_h) {
                        cimg_forC(src, c) {
                            targetSrc(x,y,0,c) = src(tx,ty,0,c);
                        }
                    }
                    isFind = true;
                    break;
                }
            }
        }

        cimg_forXY(targetTar,x,y) {
            bool isFind = false;
            for (int i = 0; i < vecTriSrc.size(); ++i) {
                if (pointInTriangle(vecTriSrc[i], Dot(x,y))) {
                    double tx = x*(*vecSrcTar[i])(0,0) + y*(*vecSrcTar[i])(0,1) + (*vecSrcTar[i])(0,2);
                    double ty = x*(*vecSrcTar[i])(1,0) + y*(*vecSrcTar[i])(1,1) + (*vecSrcTar[i])(1,2);

                    if (tx >= 0 && tx < src_w && ty >= 0 && ty < src_h) {
                        cimg_forC(target, c) {
                            targetTar(x,y,0,c) = target(tx,ty,0,c);
                        }
                    }
                    isFind = true;
                    break;
                }
            }
        }
        cimg_forXYZC(target, x, y, z, c) {
            temp(x,y,z,c) = k/RATE*targetTar(x,y,z,c)+(1-k/RATE)*targetSrc(x,y,z,c);
        }

        while(Ttemp.is_closed() != true) {
            Ttemp.wait();
            if(Ttemp.button() & 1)
                break;
        }
        temp.display(Ttemp);


    }

        /*
        if (!isFind) {
            cimg_forC(src, c) {
                targetSrc(x,y,0,c) = src.linear_atXY(x,y,0,c);
            }
        }
        */
}


Dot Ex5_FaceMorphing::getTriangleCircle(const Triangle& tri) {
    if (isThreePointOnOneLine(tri.A, tri.B, tri.C) == true)
        return Dot(-1,-1,-1);

    double x1, x2, x3, y1, y2, y3;

    x1 = tri.A.x;
    y1 = tri.A.y;
    x2 = tri.B.x;
    y2 = tri.B.y;
    x3 = tri.C.x;
    y3 = tri.C.y;

    double a = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    double b = sqrt((x1-x3)*(x1-x3)+(y1-y3)*(y1-y3));
    double c = sqrt((x2-x3)*(x2-x3)+(y2-y3)*(y2-y3));
    double p = (a+b+c)/2;
    double s = sqrt(p*(p-a)*(p-b)*(p-c));
    double radius = a*b*c/(4*s);

    double t1 = x1*x1+y1*y1;
    double t2 = x2*x2+y2*y2;
    double t3 = x3*x3+y3*y3;
    double temp=x1*y2+x2*y3+x3*y1-x1*y3-x2*y1-x3*y2;
    double x = (t2*y3+t1*y2+t3*y1-t2*y1-t3*y2-t1*y3)/2/temp;
    double y = (t3*x2+t2*x1+t1*x3-t1*x2-t2*x3-t3*x1)/2/temp;

    return Dot(x,y,radius);
}

bool Ex5_FaceMorphing::isThreePointOnOneLine(const Dot& a, const Dot& b, const Dot& c) {
    /*
    if (a.x == b.x && b.x == c.x && a.x == c.x)
        return true;

    if (a.x == b.x || b.x == c.x || a.x == c.x)
        return false;
        */
    if (b.x == a.x) {
        if (b.x == c.x)
            return true;
        return false;
    }

    //double k1 = (a.y-b.y)/(a.x-b.x);
    //double k2 = (a.y-c.y)/(a.x-c.x);

    double temp = (a.y-b.y)*(a.x-c.x)-(a.y-c.y)*(a.x-b.x);

    if (fabs(temp) < ERR)
        return true;
    return false;
}

bool Ex5_FaceMorphing::pointInCircle(const Dot& p, const Dot& center) {
    double distance = DotDisntance(p, center);
    if (distance + ERR < center.value)
        return true;
    return false;
}

double Ex5_FaceMorphing::DotDisntance(const Dot& a, const Dot& b) {
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

void Ex5_FaceMorphing::MTranstion(const double& count) {
    vecTriM.clear();
    for (int i = 0; i < vecTriSrc.size(); ++i) {
        vecTriM.push_back(middleTriangle(vecTriSrc[i], vecTriTar[i], count/RATE));
    }
}

bool Ex5_FaceMorphing::pointInTriangle(const Triangle& tri, const Dot& p) {
    Triangle pAB(p, tri.A, tri.B);
    Triangle pAC(p, tri.A, tri.C);
    Triangle pBC(p, tri.B, tri.C);

    double area_ABC = tri.ComputeArea();
    double area_pAB = pAB.ComputeArea();
    double area_pAC = pAC.ComputeArea();
    double area_pBC = pBC.ComputeArea();

    if (fabs(area_pBC +area_pAC + area_pAB - area_ABC) < ERR)
        return true;
    return false;
}
