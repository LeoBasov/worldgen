#include <vector>

namespace wg {

using namespace std;

struct Element {
    vector<double> vals;
    vector<double> grad_x;
    vector<double> grad_y;
    vector<double> grad_xx;
    vector<double> grad_xy;
    vector<double> grad_yy;
    vector<double> grad_norm;
};

struct Field {
    enum Type { ELEVATION = 0, RHO = 1 };
    enum Operation { VALUE, GRAD_X, GRAD_Y, GRAD_XX, GRAD_XY, GRAD_YY, GRAD_NORM };

    vector<Element> elements;
    int size_x = 0, size_y = 0, size = 0;

    void resize(int x, int y) {
        size_x = x;
        size_y = y;
        size = size_x * size_y;
        elements.resize(x * y);
    }

    int elem_id(int x, int y) const {
        if (x < 0 || x >= size_x || y < 0 || y >= size_y) {
            return -1;
        } else {
            return size_x * y + x;
        }
    }

    std::pair<int, int> coords(int id) const {
        if (id < 0 || id > elements.size()) {
            return {-1, -1};
        } else {
            return {id % size_x, id / size_x};
        }
    }

    double& operator()(int id, Type type, Operation operation = VALUE) {
        switch (operation) {
            case VALUE: {
                return elements.at(id).vals[type];
                break;
            }
            case GRAD_X: {
                return elements.at(id).grad_x[type];
                break;
            }
            case GRAD_Y: {
                return elements.at(id).grad_y[type];
                break;
            }
            case GRAD_XX: {
                return elements.at(id).grad_xx[type];
                break;
            }
            case GRAD_XY: {
                return elements.at(id).grad_xy[type];
                break;
            }
            case GRAD_YY: {
                return elements.at(id).grad_yy[type];
                break;
            }
            case GRAD_NORM: {
                return elements.at(id).grad_norm[type];
                break;
            }
            default: {
                throw std::exception();
                break;
            }
        }
    }

    double& operator()(int x, int y, Type type, Operation operation = VALUE) {
        return operator()(elem_id(x, y), type, operation);
    }
};

}  // namespace wg
