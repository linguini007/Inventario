#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Producto{
public:    
    int ID;
    string Nombre;
    int Cant_Stock;
    double Precio;

    Producto *siguiente;
    Producto(int ID, string Nombre, int Cant_Stock, double Precio) 
    : ID(ID),Nombre(Nombre),Cant_Stock(Cant_Stock),Precio(Precio) ,siguiente(NULL){}
};

class Inventario{
public:
    Inventario();
    ~Inventario();
    bool estaVacia()const;
    void agregarProducto(int ID, const string &Nombre, int Cant_Stock, double Precio);
    void mostrarOrdenadosPorCantidad();
    void modificarProducto(int ID, int cantidadAModificar,double nuevoPrecio);
    Producto *buscarProducto(int ID);
    void eliminarProducto(int ID);
    void guardarEnCSV(const string& nombreArchivo);
    void verificarBajoStock(int limiteBajo);
    void menu();
    private:
    Producto *primero;
};

Inventario::Inventario() : primero(NULL) {}
Inventario::~Inventario(){
    
    while (primero != NULL)
    {
        Producto *temp=primero;
        primero = primero->siguiente;
        delete temp;
    }
}
bool Inventario::estaVacia() const{
    return primero == NULL;
}
void Inventario::agregarProducto(int ID, const string &Nombre, int Cant_Stock, double Precio){
    Producto *nuevoProducto= new Producto(ID,Nombre,Cant_Stock,Precio);

    nuevoProducto->siguiente = primero;
    primero=nuevoProducto;

    cout<<"\n\tProducto agregado: "<<Nombre<<" [ID: "<<ID<<
    ",Cantidad/Stock: "<<Cant_Stock<<", Precio: "<<Precio<<"$ )\n\n";
};

void Inventario::mostrarOrdenadosPorCantidad() {
    if (estaVacia()) {
        cout << "El inventario esta vacio.\n";
        return;
    }
    Producto* listaOrdenada = nullptr;
    Producto* actual = primero;

    while (actual != nullptr) {
        Producto* nuevo = new Producto(actual->ID, actual->Nombre, actual->Cant_Stock, actual->Precio);
        if (listaOrdenada == nullptr || nuevo->Cant_Stock > listaOrdenada->Cant_Stock) {
            nuevo->siguiente = listaOrdenada;
            listaOrdenada = nuevo;
        } 
        else{
            Producto* temp = listaOrdenada;                  
            while (temp->siguiente != nullptr && temp->siguiente->Cant_Stock >= nuevo->Cant_Stock) {
                temp = temp->siguiente;
            }
            nuevo->siguiente = temp->siguiente;
            temp->siguiente = nuevo;
        }
        actual = actual->siguiente;
    }

    cout << "Productos ordenados por cantidad en stock (descendente):\n";
    Producto* temp = listaOrdenada;
    while (temp != nullptr) {
        cout << "****************************************";       
        cout << "\nID: " << temp->ID
             << "\nNombre: " << temp->Nombre
             << "\nCantidad: " << temp->Cant_Stock
             << "\nPrecio: $" << temp->Precio << "\n";
        cout << "****************************************";       
        temp = temp->siguiente;
    }
    while (listaOrdenada != nullptr) {
        Producto* eliminar = listaOrdenada;
        listaOrdenada = listaOrdenada->siguiente;
        delete eliminar;
    }
}
void Inventario::modificarProducto(int ID, int cantidadAModificar, double nuevoPrecio) {
    Producto* actual = primero;  
    while (actual != nullptr) {
        if (actual->ID == ID) {
            if (cantidadAModificar < 0) {
                if (actual->Cant_Stock >= -cantidadAModificar) {
                    actual->Cant_Stock += cantidadAModificar;

                    cout << "\n\nSe han eliminado " << -cantidadAModificar << " unidades del producto " 
                    << actual->Nombre << ". Nuevo stock: " << actual->Cant_Stock << "\n\n";

                } else {
                    cout << "\n\nNo hay suficiente stock para eliminar.\n";
                }
            } else if (cantidadAModificar > 0) { 
                actual->Cant_Stock += cantidadAModificar;

                cout << "\n\nSe han agregado " << cantidadAModificar << " unidades al producto " 
                << actual->Nombre << ". Nuevo stock: " << actual->Cant_Stock << "\n\n";
            }
            if (nuevoPrecio > 0) {
                actual->Precio = nuevoPrecio;
                cout << "\n\nEl precio del producto " << actual->Nombre << 
                " ha sido actualizado a: " << actual->Precio << " $\n\n";
            }
            return;
        }
        actual = actual->siguiente;  
    }
    cout << "\n\tProducto con ID " << ID << " no encontrado.\n";
}
Producto *Inventario::buscarProducto(int ID) {
    Producto* actual = primero; 
    while (actual != nullptr) {
        if (actual->ID == ID) { 
            return actual; 
        }
        actual = actual->siguiente; 
    }
    return nullptr;
}
void Inventario::eliminarProducto(int ID) {
    if (estaVacia()) {
        cout << "\n\tEl inventario está vacío. No se puede eliminar el producto.\n";
        return;
    }
    if (primero != nullptr && primero->ID == ID) {
        Producto* temp = primero;
        primero = primero->siguiente; 
        delete temp; 
        cout << "Producto con ID " << ID << " eliminado.\n";
        return;
    }
    Producto* actual = primero;
    Producto* anterior = nullptr;
    while (actual != nullptr) {
        if (actual->ID == ID) {
            anterior->siguiente = actual->siguiente;
            delete actual; 
            cout << "Producto con ID " << ID << " eliminado.\n";
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    cout << "Producto con ID " << ID << " no encontrado en el inventario.\n";
}
void Inventario::guardarEnCSV(const string &nombreArchivo) {
    ofstream archivo(nombreArchivo); // Abrir archivo en modo escritura
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo para escribir.\n";
        return;
    }

    // Escribir encabezados
    archivo << "ID,Nombre,Cantidad,Precio\n";
    // Recorrer la lista y escribir los productos
    Producto* actual = primero;

    while (actual != nullptr) {
        archivo << actual->ID << ","
                << actual->Nombre << ","
                << actual->Cant_Stock << ","
                << actual->Precio << "\n";
        actual = actual->siguiente;
    }
    archivo.close(); // Cerrar el archivo
    cout << "Inventario guardado en el archivo '" << nombreArchivo << "'.\n";
}

void Inventario::verificarBajoStock(int limiteBajo){
     Producto *actual = primero;
    bool hayBajoStock = false;
    if(estaVacia()){
        system("cls");
        system("color c");
        cout<<"\n\n\n\n\t\t>>>>>NO HAY PRODUCTOS GUARDADOS<<<<<\n\n";
        return;
    }
    cout<<"\n>>Estos son los Productos con bajo Stock.(Por debajo de "
    <<limiteBajo<<" Unidades del producto)<<\n";
    while (actual != NULL){
        if(actual->Cant_Stock <= limiteBajo){
            system("color c");
                 cout << "Producto: " << actual->Nombre 
                 << " [ID: " << actual->ID 
                 << ", Cantidad en stock: " << actual->Cant_Stock 
                 << ", Precio: " << actual->Precio << "$]\n";
            hayBajoStock = true;
        }
        actual = actual->siguiente;
    }
    if(!hayBajoStock){
        system("cls");
        cout << "\n\n\tNo hay productos con bajo stock.\n";

    }
}

void Inventario::menu() {
    int opcion;
    int id, cantidad, cantidadAEliminar;
    double precio;
    string nombre;
    Producto* producto = nullptr;

    do {
        system("color F");
        cout << "\n >>Menu de Opciones<<\n";
        cout << "[0] Salir\n";
        cout << "[1] Agregar producto\n";
        cout << "[2] Eliminar producto\n";
        cout << "[3] Modificar producto\n";
        cout << "[4] Mostrar productos ordenados por cantidad\n";
        cout << "[5] Buscar producto por ID\n";
        cout << "[6] Verifica los Stocks BAJOS\n";
        cout << "[7] GUARDAR LOS DATOS\n";
        cout << "[8] Limpiar pantalla\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        switch (opcion){
            case 0:
                break;
            case 1:
                system("cls");
                cout << "Ingrese ID: ";
                cin >> id;
                cout << "Ingrese Nombre: ";
                cin >> ws;  //Limpia el buffer para las entradas nuevas
                getline(cin, nombre);  // Permite leer entradas con espacios 
                cout << "Ingrese cantidad en stock: ";
                cin >> cantidad;
                cout << "Ingrese precio: ";
                cin >> precio;
                agregarProducto(id, nombre, cantidad, precio);
                system("pause");
                system("cls");
                break;
            case 2:
                cout << "Ingrese ID del producto a eliminar: ";
                cin >> id;
                eliminarProducto(id);
                break;

            case 3:
                system("cls");
                cout << "Ingrese ID del producto a modificar: ";
                cin >> id;
                cout << "Ingrese cantidad a modificar (-5 resta o 5 suma ): ";
                cin >> cantidadAEliminar;
                cout << "Ingrese nuevo precio: ";
                cin >> precio;
                modificarProducto(id, cantidadAEliminar, precio);
                system("pause");
                system("cls");
                break;
            case 4:
                system("cls");
                mostrarOrdenadosPorCantidad();
                cout<<"\n";
                system("pause");
                break;

            case 5:
                system("cls");
                cout << "Ingrese ID del producto a buscar: ";
                cin >> id;
                producto = buscarProducto(id);
                if (producto) {
                    cout << "Producto encontrado: " << producto->Nombre << " - Cantidad: " << producto->Cant_Stock 
                    << " - Precio: " << producto->Precio << "$\n";
                } else {
                    cout << "Producto no encontrado.\n";
                }
                system("pause");
                system("cls");
                break;

            case 6:
                system("cls");
                int limiteBajo;
                cout << "Ingrese el limite bajo para verificar bajo stock: ";
                cin >> limiteBajo;
                verificarBajoStock(limiteBajo);
                system("pause");
                system("cls");
                break;

            case 7:
                system("cls");
                guardarEnCSV("________________________RUTA DE GUARDADO__________________________");
                system("pause");
                system("cls");
                break;

            case 8:
            system("cls");
                break;

            default:   
                system("cls");
                cout << "Opcion no valida, intente nuevamente.\n";
                system("pause");
                system("cls");
                break;
        }
    } while (opcion != 0);
}
int main()
{
    system("cls");
    Inventario almacen;
    almacen.menu();

    
   
    cout <<"\n\n\n";
    return 0;
}



