#include <iostream>

class Rectangle{
    private:
        int width;
        int height;
    public:
        Rectangle(int w, int h){
            width = w;
            height = h;
        }      

        int getHeight() const{
            return height;
        }

        int getWidth() const{
            return width;
        } 

        int getPerimeter(){
            return 2 * (width + height);
        }

        int getArea(){
            return width * height;
        }
};

int main(){
    Rectangle r(10, 20);
    std::cout << r.getPerimeter() << std::endl;
    std::cout << r.getArea() << std::endl;
    int n;
    std::cin >> n;
    std::cout << n << std::endl;
}
