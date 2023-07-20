#include <iostream>
#include <string>
#include <fstream>
#include <vector>

// Структура хранящая информацию о точке, перегрузка операторов помещения и взяти из потока.
struct Point {
    int x,y;
    Point() {};
    Point(int x,int y): x(x), y(y) {};

    friend std::istream& operator>>(std::istream& in, Point& p) {
        // temp скипает запятую при считывании
        char temp;
        in >> p.x >> temp >> p.y;
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, Point& p) {
        out << p.x << ',' << p.y;
        return out;
    }
};

// Функция определяющая в какой из полуплоскостей находится точка
int getOrientation(Point p, Point q, Point r) {
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;  // Коллинеарны
    return (val > 0) ? 1 : -1; // 1 - По часовой стрелке, -1 - Против часовой стрелки
}

int main() {
    std::string polys_dir, lines_dir, results_dir;

    std::cout << "Specify the directory of the file \"polys.txt\"" << std::endl;
    std::cin >> polys_dir;

    std::cout << "Specify the directory of the file \"lines.txt\"" << std::endl;
    std::cin >> lines_dir;

    std::ifstream finp(polys_dir);
    std::ifstream finl(lines_dir);

    std::vector<Point> vertices;
    Point vertex;
    while (finp >> vertex) vertices.push_back(vertex);

    std::vector<std::pair<Point, Point>> segments;
    Point p1,p2;
    while (finl >> p1 >> p2) segments.push_back({p1,p2});

    finp.close();
    finl.close();

    std::cout << "Specify the directory of the file \"results.txt\"" << std::endl;
    std::cin >> results_dir;

    std::ofstream fout(results_dir);

    // проверяем лежат ли точки внутри N угольника
    for (std::pair<Point, Point> pa : segments) {
        bool first = 1, second = 1;
        int orientation, nextOrientation;
        // проверки, думаю, можно написать лучше
        orientation = getOrientation(vertices.back(), vertices.front(), pa.first);
        for (int i=0;i<vertices.size()-1;++i) {
            nextOrientation = getOrientation(vertices.at(i), vertices.at(i+1), pa.first);
            if (nextOrientation == 0) continue;
            if (orientation == 0) orientation = nextOrientation;
            first = first & (orientation == nextOrientation);
        }
        orientation = getOrientation(vertices.back(), vertices.front(), pa.second);
        for (int i=0;i<vertices.size()-1;++i) {
            nextOrientation = getOrientation(vertices.at(i), vertices.at(i+1), pa.second);
            if (nextOrientation == 0) continue;
            if (orientation == 0) orientation = nextOrientation;
            second = second & (orientation == nextOrientation);
        }
        if (first&&second) {
            fout << 0 << std::endl;
        } else if (first||second) {
            fout << 2 << std::endl;
        } else {
            orientation = getOrientation(pa.first, pa.second, vertices.at(0));
            for (int i=1;i<vertices.size();++i) {
                nextOrientation = getOrientation(pa.first, pa.second, vertices.at(i));
                if (orientation != nextOrientation) {
                    if (getOrientation(vertices.at(0), vertices.at(1), pa.first) !=
                        getOrientation(vertices.at(0), vertices.at(1), pa.second)) {
                        fout << 1 << std::endl;
                        break;
                    }
                }
                if(i==vertices.size()-1) fout << 3 << std::endl;
            }
        }
    }

    fout.close();
    return 0;
}