#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "list.h"

typedef struct{
    char name[31];
    char brand[31];
    char type[31];
    int stock;
    int price;
} Item;

typedef struct{
    char name[31];
    List * itemList;
} ListOfTypesOrBrands;

typedef struct{
    char name[31];
    int quantity;
} CartNode;

typedef struct{
    char cartName[31];
    List * itemList;
} Cart;


typedef struct{
    Map * mapNames;
    Map * mapBrands;
    Map * mapTypes;
    Map * mapCarts;
} Maps;


//Función para tomar un string de una coordenada específica de un archivo .csv
const char *get_csv_field (char * tmp, int k) {
    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0' && tmp[i+1]!='\n'){

        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }

        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }

    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }


    return NULL;
}

//Función para comparar claves de tipo string, retorna 1 si son iguales
int is_equal_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

void pressEnterToContinue()
{
    printf("\n(Presiona Enter para continuar.)\n");
    char enter = 0;
    while (enter != '\r' && enter != '\n')
    {
        enter = getchar();
    }
}

CartNode * searchCartItem(List * cart, char * itemName)
{
    CartNode * cartNode = firstList(cart);
    while(is_equal_string(cartNode->name, itemName) == 0)
    {
        cartNode = nextList(cart);
        if(cartNode == NULL)
        {
            return NULL;
        }
    }
    return cartNode;
}

//Ingresa el producto a los Mapas
void insertItem(Maps * maps, Item * importedItem)
{
    //Inserta en el mapa por Nombres
    insertMap(maps->mapNames, importedItem->name, importedItem);

    //Busca si la marca ya existe
    ListOfTypesOrBrands * auxList = searchMap(maps->mapBrands, importedItem->brand);
    //Si no es el caso, crea la Lista
    if(auxList == NULL)
    {
        //Crea dato Auxiliar
        auxList = (ListOfTypesOrBrands *) malloc (sizeof(ListOfTypesOrBrands));    
        //Le asigna el nombre de la marca al dato
        strcpy(auxList->name, importedItem->brand);
        //Inicializa la Lista del dato (Lista de Productos con esta marca)
        auxList->itemList = createList();
        //Inserta en el mapa por Marcas
        insertMap(maps->mapBrands, importedItem->brand, auxList);
    }
    //Añade el producto a la lista
    pushBack(auxList->itemList, importedItem->name);

    //Busca si el tipo ya existe
    auxList = searchMap(maps->mapTypes, importedItem->type);
    //Si no es el caso, crea la Lista
    if(auxList == NULL)
    {
        //Crea dato Auxiliar
        auxList = (ListOfTypesOrBrands *) malloc (sizeof(ListOfTypesOrBrands));    
        //Le asigna el nombre de la marca al dato
        strcpy(auxList->name, importedItem->type);
        //Inicializa la Lista del dato (Lista de Productos con esta marca)
        auxList->itemList = createList();
        //Inserta en el mapa por Marcas
        insertMap(maps->mapTypes, importedItem->type, auxList);
    }
    //Añade el producto a la lista
    pushBack(auxList->itemList, importedItem->name);
}

//Carga las canciones desde un archivo .csv y las ingresa a sus respectivas listas
void readItemsFromCSV(FILE * csvFile, Maps * maps)
{
    char linea[1024];

    while(fgets(linea, 1023, csvFile) != NULL)
    {
        Item * importedItem = (Item *)malloc(sizeof(Item));

        if(importedItem == NULL)
        {
            printf("Error al almacenar memoria para crear el Producto.");
            exit(1);
        }
        else
        {
            printf("Producto creado correctamente\n");
        }
        
        strcpy(importedItem->name, get_csv_field(linea, 0));
        //importedItem->name = get_csv_field(linea, 0);
        printf("Nombre: %s\n", importedItem->name);

        Item * auxNode = searchMap(maps->mapNames, importedItem->name);
        if(auxNode != NULL)
        {
            auxNode->stock+= importedItem->stock;
        }
        else
        {
            strcpy(importedItem->brand, get_csv_field(linea, 1));
            //importedItem->brand = get_csv_field(linea, 1);
            printf("Marca: %s\n", importedItem->brand);

            strcpy(importedItem->type, get_csv_field(linea, 2));
            //importedItem->type = get_csv_field(linea, 2);
            printf("Tipo: %s\n", importedItem->type);

            importedItem->stock = atoi(get_csv_field(linea, 3));
            printf("Cantidad: %i\n", importedItem->stock);

            importedItem->price = atoi(get_csv_field(linea, 4));
            printf("Precio: $%i\n\n", importedItem->price);

            insertItem(maps, importedItem);
        }
    }
}

void createItem(Maps * maps)
{
    printf("Creando un objeto manualmente\n");

    Item * newItem = (Item *)malloc(sizeof(Item));

    if(newItem == NULL)
    {
        printf("Error al almacenar memoria para el producto, saliendo del programa.");
        exit(1);
    }
    
    printf("Ingrese el nombre del producto (Respete mayúsculas):\n");
    getchar();
    gets(newItem->name);
    printf("Nombre ingresado: %s\n", newItem->name);

    Item * auxNode = searchMap(maps->mapNames, newItem->name);
    if(auxNode != NULL)
    {
        printf("El producto ya existe, desea cambiar el stock del mismo?\n1.- Sí\n2.- No\n\n");
        int p;
        scanf("%i", &p);
        if(p == 1)
        {
            printf("Ingrese la variación del stock (+N/-N):\n");
            scanf("%i", &newItem->stock);
            auxNode->stock+= newItem->stock;
            printf("El nuevo stock es de %i\n", auxNode->stock);
        }
        else
        {
            printf("Volviendo al menú.\n");
            return;
        }
    }
    else
    {
        printf("Ingrese la marca del producto:\n");
        gets(newItem->brand);

        printf("Ingrese el tipo de producto:\n");
        gets(newItem->type);

        printf("Ingrese la cantidad inicial del producto:\n");
        scanf("%i", &newItem->stock);

        printf("Ingrese el valor del producto:\n");
        scanf("%i", &newItem->price);

        insertItem(maps, newItem);
        printf("Producto creado correctamente");
    }
}

void searchByBrandOrType(Map * map, Map * mapNames)
{
    char key[31];

    printf("Ingrese la clave a buscar:\n");
    getchar();
    gets(key);
    printf("Buscando productos de %s\n\n", key);

    ListOfTypesOrBrands * auxNode = searchMap(map, key);
    if(auxNode)
    {
        char * aux = firstList(auxNode->itemList);

        printf("Productos %s\n------------------------------\n\n", key);

        Item * auxItem;

        //Recorrer la lista
        while(aux != NULL)
        {
            //Imprimir los datos del producto
            auxItem = searchMap(mapNames, aux);

            printf("Nombre: %s\n", auxItem->name);
            printf("Marca: %s\n", auxItem->brand);
            printf("Tipo: %s\n", auxItem->type);
            printf("Stock: %i\n", auxItem->stock);
            printf("Precio: $%i\n\n", auxItem->price);
            
            aux = nextList(auxNode->itemList);
            if(aux == NULL)
            {
                break;
            }
        }
    }
    else
    {
        printf("No existen productos con esas características.\nVolviendo al menú\n");
        return;
    }
}

void searchByName(Map * map)
{
    char key[31];

    printf("Ingrese el nombre del producto a buscar:\n");
    getchar();
    gets(key);

    Item * auxNode = searchMap(map, key);
    if(auxNode)
    {
        printf("Nombre: %s\n", auxNode->name);
        printf("Marca: %s\n", auxNode->brand);
        printf("Tipo: %s\n", auxNode->type);
        printf("Cantidad: %i\n", auxNode->stock);
        printf("Precio: $%i\n", auxNode->price);
    }
    else
    {
        printf("No existe un producto con ese nombre.\nVolviendo al menú\n");
        return;
    }
}

void showAllItems(Map * map)
{
    Item * auxNode = firstMap(map);

    printf("Mostrando todos los productos disponibles.\n");

    while (auxNode)
    {
        printf("Nombre: %s\n", auxNode->name);
        printf("Marca: %s\n", auxNode->brand);
        printf("Tipo: %s\n", auxNode->type);
        printf("Cantidad: %i\n", auxNode->stock);
        printf("Precio: $%i\n\n", auxNode->price);
        auxNode = nextMap(map);
    }

    printf("Mostrar los productos ha finalizado.\nVolviendo al menú.\n");
}

void addToCart(Maps * maps)
{
    char itemName[31];
    char cartName[31];
    printf("Ingrese el nombre del producto que desea agregar.\n");
    getchar();
    gets(itemName);

    //Crea un Producto auxiliar para buscar al Producto en la lista por nombres
    Item * auxItem = searchMap(maps->mapNames, itemName);
    //Si no lo encuentra vuelve al menú
    if(auxItem == NULL)
    {
        printf("El producto no existe.\nVolviendo al menú.\n");
        return;
    }
    //Si no queda stock vuelve al menú
    if(auxItem->stock <= 0)
    {
        printf("El producto se encuentra sin stock.\nVolviendo al menú.\n");
        return;
    }

    //Crea un nuevo nodo de Producto para el Carrito
    CartNode * newCartItem = (CartNode *) malloc (sizeof(CartNode));
    //Asigna el nombre del Producto al nodo
    strcpy(newCartItem->name, itemName);

    printf("Ingrese la cantidad que desea añadir al carrito. (Actualmente disponible: %i unidades)\n", auxItem->stock);
    scanf("%i", &newCartItem->quantity);
    //Si se ingresa un valor mayor al disponible, pedirá ingresar denuevo
    while(newCartItem->quantity > auxItem->stock /*|| newCartItem->quantity < 0*/)
    {
        printf("No hay suficientes unidades, por favor ingrese una cifra menor o igual al stock (%i)\n", auxItem->stock);
        scanf("%i", &newCartItem->quantity);
    }

    printf("Ingrese el nombre del carrito al que desea agregar %i unidades de %s.\n", newCartItem->quantity, newCartItem->name);
    getchar();
    gets(cartName);
    //Busca si el carrito existe
    Cart * auxCart = searchMap(maps->mapCarts, cartName);
    //Si no existe, se pregunta si se desea crearlo (en caso de haberse equivocado de nombre)
    if(auxCart == NULL || strcmp(cartName, auxCart->cartName) == 1)
    {
        printf("El carrito %s no existe, desea crearlo?\n1.- Sí\n2.- No\n\n", cartName);
        int no = 0;
        while(no != 1 && no != 2)
        {
            scanf("%i", &no);
            if(no == 2)
            {
                printf("Volviendo al menú\n");
                return;
            }
        }
        //Se reserva memoria para el carrito
        auxCart = (Cart *) malloc (sizeof(Cart));
        strcpy(auxCart->cartName, cartName);
        //Si se desea crearlo, se crea una Lista (carrito)
        auxCart->itemList = createList();
        //Se agrega el Producto al carrito
        pushBack(auxCart->itemList, newCartItem);
        //El carrito se ingresa en el mapa con clave "nombre del carrito"
        insertMap(maps->mapCarts, auxCart->cartName, auxCart);
    }
    else
    {
        printf("Buscando producto en el carrito\n");
        //Se busca el producto en el carrito
        CartNode * auxCartNode = searchCartItem(auxCart->itemList, itemName);
        //Si el producto no estaba en el carrito
        if(auxCartNode == NULL)
        {
            printf("El producto no se encuentra en el carrito\n");
            //Se añade el producto al carrito
            pushBack(auxCart->itemList, newCartItem);
        }
        else
        {
            printf("El producto si existe en el carrito, sumando la cantidad\n");
            //Se incrementa la cantidad del producto en el carrito
            auxCartNode->quantity += newCartItem->quantity;
        }
    }
    //Se reduce la disponibilidad del producto
    auxItem->stock-=newCartItem->quantity;
    printf("Se ha agregado el producto al carrito %s.\n", cartName);
}

void removeProductFromCart(Map * map, Map * mapNames)
{
    Cart * cart = firstMap(map);
    //Si no hay carritos, volver al menú
    if(cart == NULL)
    {
        printf("Actualmente no existe ningún carrito.\nVolviendo al menú.\n");
        return;
    }
    char cartName[31];
    printf("Ingrese el nombre del carrito al que desea remover el último producto:\n");
    getchar();
    gets(cartName);
    cart = searchMap(map, cartName);
    if(cart == NULL)
    {
        printf("El carrito no existe, volviendo al menú.\n");
        return;
    }
    printf("Removiendo el último elemento agregado a %s\n", cartName);

    CartNode * cartNode = lastList(cart->itemList);

    Item * item = searchMap(mapNames, cartNode->name);
    item->stock += cartNode->quantity;
    
    popBack(cart->itemList);
    
    printf("Producto eliminado.\n");
}

void pay(Maps * maps)
{
    Cart * cart = firstMap(maps->mapCarts);
    //Si no hay carritos, volver al menú
    if(cart == NULL)
    {
        printf("Actualmente no existe ningún carrito.\nVolviendo al menú.\n");
        return;
    }
    char cartName[31];
    printf("Ingrese el nombre del carrito que desea pagar:\n");
    getchar();
    gets(cartName);
    cart = searchMap(maps->mapCarts, cartName);
    if(cart == NULL)
    {
        printf("El carrito no existe, volviendo al menú.\n");
        return;
    }
   
    Item * auxItem;
    int totalCart = 0;
    //Mientras el carrito actual exista
    CartNode * auxCartNode = firstList(cart->itemList);
    printf("Carrito: %s\n------------------------------\n", cart->cartName);
    //Recorrer la lista del carrito
    while(auxCartNode != NULL)
    {
        auxItem = searchMap(maps->mapNames, auxCartNode->name);
        totalCart += auxItem->price * auxCartNode->quantity;
        auxCartNode = nextList(cart->itemList);
        if(auxCartNode == NULL)
        {
            printf("Total a pagar del carrito %s: $%i\n\n", cart->cartName, totalCart);
            break;
        }
    }
    printf("Con cuanto dinero va a pagar?\n");
    int payment = 0;
    int tries = 0;
    scanf("%i", &payment);

    while(payment < totalCart)
    {
        tries++;
        if(tries > 3)
        {
            printf("Demasiados intentos, volviendo al menú.\n");
            return;
        }
        printf("El dinero no es suficiente, ingrese un valor mayor o igual al total.\n");
        scanf("%i", &payment);
    }
    payment -= totalCart;
    printf("Gracias por su compra");
    if(payment != 0) printf(", su vuelto es $%i\n", payment);
    cleanList(cart->itemList);
    free(cart->cartName);
    eraseMap(maps->mapCarts, cartName);
}

void showCarts(Maps * maps)
{
    Cart * auxCart = firstMap(maps->mapCarts);
    //Si no hay carritos, volver al menú
    if(auxCart == NULL)
    {
        printf("Actualmente no existe ningún carrito.\nVolviendo al menú.\n");
        return;
    }

    printf("Mostrando los carritos actuales.\n\n");
    Item * auxItem;
    int totalProduct = 0;
    int totalCart = 0;
    //Mientras el carrito actual exista
    while(auxCart != NULL)
    {
        CartNode * auxCartNode = firstList(auxCart->itemList);
        printf("Carrito: %s\n------------------------------\n", auxCart->cartName);
        totalCart = 0;
        //Recorrer la lista del carrito
        while(auxCartNode != NULL)
        {
            //Imprimir los datos del producto
            auxItem = searchMap(maps->mapNames, auxCartNode->name);

            printf("Nombre: %s\n", auxItem->name);
            printf("Marca: %s\n", auxItem->brand);
            printf("Tipo: %s\n", auxItem->type);
            printf("Precio: $%i\n", auxItem->price);
            printf("Cantidad: %i\n", auxCartNode->quantity);

            totalProduct = auxItem->price * auxCartNode->quantity;
            printf("Precio total: $%i\n\n", totalProduct);
            totalCart+=totalProduct;

            auxCartNode = nextList(auxCart->itemList);
            if(auxCartNode == NULL)
            {
                printf("Total del carrito: $%i\n\n", totalCart);
                break;
            }
        }
        //Pasar al siguiente carrito
        auxCart = nextMap(maps->mapCarts);
        if(auxCart == NULL) break;
    }
}

//MAIN----------MAIN----------MAIN----------MAIN----------MAIN----------MAIN----------MAIN----------MAIN----------MAIN----------MAIN
//-------MAIN----------MAIN----------MAIN----------MAIN----------MAIN----------MAIN----------MAIN----------MAIN----------MAIN-------

int main()
{
    //Valor usado en el menú para saber que opción se quiere realizar
    int op = 0;
    Maps * maps = (Maps *) malloc (sizeof(Maps));

    //Marca el inicio del programa para el usuario
    printf("INICIO\n");

    //Crear Mapa con clave: nombre
    maps->mapNames = createMap(is_equal_string);

    //Crear Mapa con clave: marca
    maps->mapBrands = createMap(is_equal_string);

    //Crear Mapa con clave: nombre
    maps->mapTypes = createMap(is_equal_string);

    //Crear Mapa para de carritos
    maps->mapCarts = createMap(is_equal_string);

    while(op!=12)
    {
        pressEnterToContinue();
        printf("\n--------------------MENU--------------------\n");
        printf("1.- Importar productos desde un archivo CSV\n");
        printf("2.- Exportar productos a un archivo CSV\n");
        printf("3.- Agregar producto\n");
        printf("4.- Buscar producto por nombre\n");
        printf("5.- Buscar productos por marca\n");
        printf("6.- Buscar productos por tipo\n");
        printf("7.- Mostrar todos los productos\n");
        printf("8.- Agregar al carrito\n");
        printf("9.- Eliminar del carrito\n");
        printf("10.- Concretar compra\n");
        printf("11.- Mostrar carritos de compra\n");
        printf("12.- SALIR\n");

        printf("--------------------------------------------\nIndica la opción:\n");
        scanf("%i", &op);
        printf("Opcion escojida = %i\n\n", op);

        switch(op)
        {
            case 1: //Lee un archivo .csv y almacena los productos en el mapa
            {
                char nombreArchivo[100];
                printf("Ingrese el nombre del archivo que desea importar (ingrese 0 si quiere cargar el archivo por defecto):\n");
                scanf("%s", nombreArchivo);

                if(is_equal_string(nombreArchivo, "0") == 1)
                {
                    strcpy(nombreArchivo, "Archivo_100productos.csv");
                }

                printf("Leyendo archivo: %s\n", nombreArchivo);
                FILE *csvFile = fopen(nombreArchivo, "r");
                if (csvFile == NULL)
                {
                    printf("Error al abrir el archivo.\n");
                    break;
                }
                printf("\nArchivo %s cargado Correctamente\n\n", nombreArchivo);

                readItemsFromCSV(csvFile, maps);

                printf("\nProductos ingresados Correctamente\n");

                break;
            }
            case 2:
            {
                printf("Esta opción no está disponible actualmente.\n");
                break;
            }
            case 3:
            {
                /*Agregar producto (nombre, marca, tipo, stock, precio):​La
                aplicación deberá crear un producto nuevo, validando que el
                producto no exista previamente. Si el producto ya existe tendrá
                que aumentar el stock disponible en la cantidad indicada.*/

                createItem(maps);
                break;
            }
            case 4:
            {
                /*Buscar producto por nombre (nombre):​ La aplicación busca y
                muestra por pantalla el producto (y su respectiva información),
                de no existir el producto, debe mostrar un mensaje por pantalla.*/

                searchByName(maps->mapNames);
                break;
            }
            case 5:
            {
                /*Buscar productos por marca (marca):​ La aplicación busca y
                muestra por pantalla todos los productos (y su respectiva
                información) de la marca especificada, de no existir la marca,
                debe mostrar un mensaje por pantalla*/
                
                searchByBrandOrType(maps->mapBrands, maps->mapNames);
                break;
            }
            case 6:
            {
                /*Buscar productos por tipo (tipo):​ La aplicación busca y
                muestra por pantalla todos los productos (y su respectiva
                información) del tipo especificado, de no existir el tipo,
                debe mostrar un mensaje por pantalla.*/

                searchByBrandOrType(maps->mapTypes, maps->mapNames);
                break;
            }
            case 7:
            {
                /*Mostrar todos los productos ():​ La aplicación muestra por
                pantalla todos los productos existentes (y su respectiva
                información).*/

                showAllItems(maps->mapNames);
                break;
            }
            case 8:
            {
                /*Agregar al carrito(nombre_producto, cantidad, nombre_carrito):
                La aplicación agrega el producto indicado al carrito de compras
                correspondiente. Si el carrito no existe se crea uno nuevo.*/

                addToCart(maps);
                break;
            }
            case 9:
            {
                /*Eliminar del carrito (nombre_carrito): Se elimina el último
                producto ingresado al carrito.*/
                removeProductFromCart(maps->mapCarts, maps->mapNames);
                break;
            }
            case 10:
            {
                /*Concretar compra (nombre_carrito): La aplicación muestra el
                total a pagar por los productos del carrito, muestra los
                productos en el orden de ingreso y resta del stock los
                productos correspondientes. El carrito de compra es eliminado.*/
                pay(maps);
                break;
            }
            case 11:
            {
                /*Mostrar carritos de compra: Se muestran los nombres de los
                carritos de compra creados y la cantidad de productos que
                tiene cada uno de ellos.*/
                showCarts(maps);
                break;
            }
            case 12:
            {
                printf("Saliendo del programa...");
                break;
            }
        }
    }
    return 0;
}