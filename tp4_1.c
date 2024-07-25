#include <stdio.h>//contiene las funciones de entrada salida desde la consola
#include <stdlib.h>
#include <string.h>

struct{
    int TareaID;//Numérico autoincremental comenzando en 1000
    char *Descripcion; //
    int Duracion; // entre 10 – 100
}typedef Tarea;

typedef struct Nodo{
    Tarea T;
    struct Nodo *Siguiente;
}Nodo;

Nodo *crearListaVacia();
Nodo *crearNodo(Tarea tarea);
void insertarNodo(Nodo ** lista, Nodo *nuevoNodo);
void cargarPendientes(Nodo ** pendientes); //(pendientes o lista es lo mismo)
void mostrarTarea(Tarea tarea);
void mostrarLista(Nodo *lista);
void eliminarYmover(Nodo ** pendientes, Nodo ** realizadas, int id);
void moverHaciaRealizadas(Nodo **pendientes, Nodo **realizadas);
Nodo *buscarNodoId(Nodo ** lista , int dato);
Nodo *buscarNodoPalabra(Nodo ** lista , char *palabra);
void consultarTareas(Nodo **pendientes, Nodo **realizadas);
void liberarLista(Nodo *lista);

int main(){
    Nodo * pendientes = crearListaVacia();
    Nodo * realizadas = crearListaVacia();

    //cargar la lista de tareas pendientes
    cargarPendientes(&pendientes);

    //elegir tarea a mover
    moverHaciaRealizadas(&pendientes, &realizadas);

    //mostrar listas
    printf("Lista de Tareas Pendientes: \n");
    mostrarLista(pendientes);
    printf("\n");
    printf("Lista de Tareas Realizadas: \n");
    mostrarLista(realizadas);    

    //consultar tareas segun id o palabra
    consultarTareas(&pendientes, &realizadas);

    // liberar memoria
    liberarLista(pendientes);
    liberarLista(realizadas);

    return 0;
}

Nodo * crearListaVacia()
{
    return NULL;
}

Nodo *crearNodo(Tarea tarea)
{
    Nodo *nodo = (Nodo *)malloc(sizeof(Nodo));

    nodo->T.TareaID = tarea.TareaID;

    nodo->T.Descripcion = (char *)malloc((strlen(tarea.Descripcion) + 1) * sizeof(char));
    strcpy(nodo->T.Descripcion, tarea.Descripcion);

    nodo->T.Duracion = tarea.Duracion;

    nodo->Siguiente = NULL;
    return nodo;
}

void insertarNodo(Nodo **lista, Nodo *nuevoNodo)
{
    nuevoNodo->Siguiente = *lista;
    *lista  = nuevoNodo ;
}

void cargarPendientes(Nodo **pendientes)
{
    char opcion; //s = si, n = no
    int id = 1000;
    do
    {
        printf("=====> Ingresando tarea/s pendientes \n");

        Tarea nuevaTarea;
        nuevaTarea.TareaID = id++;
        nuevaTarea.Descripcion = (char *)malloc(100 * sizeof(char));

        printf("Ingrese la descripcion: \n");
        gets(nuevaTarea.Descripcion);

        printf("Ingrese la duracion: \n");
        scanf("%d", &nuevaTarea.Duracion);
        fflush(stdin);

        insertarNodo(pendientes, crearNodo(nuevaTarea)); //creo el nodo y lo inserto en un mismo paso

        printf("Desea ingresar una nueva tarea? (Presione 's' para SI, o 'n' para NO) \n");
        scanf(" %c", &opcion);
        fflush(stdin);
        
    } while (opcion == 'S' || opcion == 's');

}

void mostrarTarea(Tarea tarea)
{
    printf("=====================================\n");
    printf("ID de Tarea: %d \n", tarea.TareaID);
    printf("Descripcion: %s\n", tarea.Descripcion);
    printf("Duracion: %d \n", tarea.Duracion);
    printf("=====================================\n");
}

void mostrarLista(Nodo *lista)
{
    int i = 0;
    while (lista != NULL)
    {
        printf("    TAREA [%d] \n", i++);
        mostrarTarea(lista->T);
        lista = lista->Siguiente;
    }
}

void eliminarYmover(Nodo ** pendientes, Nodo ** realizadas, int id)
{
    Nodo **aux = pendientes;
    //itero hasta encontrar el id buscado
    while (*aux != NULL && (*aux)->T.TareaID != id)
    {
        aux = &(*aux)->Siguiente;
    }

    if (*aux) //si encuentro...
    {
        Nodo *temp = *aux; //guardo el nodo a eliminar en una var temporal
        *aux = (*aux)->Siguiente; //desvinculo el nodo de la lista
        insertarNodo(realizadas, temp);

        printf("Se agrego correctamente la tarea de ID %d a la lista de realizadas. \n", id);
        //free(temp);//libero memoria ocupada por el nodo
    }else {
        printf("No se encontro una tarea con el ID %d. \n", id);
    } 
  
}

void moverHaciaRealizadas(Nodo **pendientes, Nodo **realizadas)
{
    int buscado;
    //muestro la lista de pendientes
    printf("Lista de Tareas Pendientes: \n");
    mostrarLista(*pendientes);

    printf("Ingrese el ID de la tarea pendiente que desea mover: \n");
    scanf("%d", &buscado);
    fflush(stdin);

    eliminarYmover(pendientes, realizadas, buscado);

}

Nodo *buscarNodoId(Nodo ** lista , int dato)
{
  Nodo *Aux = *lista;
  while (Aux && Aux->T.TareaID != dato)
  {
    Aux = Aux->Siguiente;
  }
  return Aux;
}

Nodo *buscarNodoPalabra(Nodo ** lista , char *palabra)
{
  Nodo *Aux = *lista;
  while (Aux && strcmp(palabra, Aux->T.Descripcion) != 0)
  {
    Aux = Aux->Siguiente;
  }
  return Aux;
}


void consultarTareas(Nodo **pendientes, Nodo **realizadas)
{
    int opcion, id; //opcion 1 para buscar por ID, y 2 para buscar por palabra clave
    char palabra[100];

    printf("Ingrese de que forma desea consultar la tarea: \n");
    printf("1: por ID. \n");
    printf("2: por palabra clave. \n");
    scanf("%d", &opcion);

    switch (opcion)
    {
    case 1:
        printf("Ingrese el ID: \n");
        scanf("%d", &id);
        fflush(stdin);

        Nodo *resultadoIdPendientes = buscarNodoId(pendientes, id); //buscado en pendientes
        Nodo *resultadoIdRealizadas = buscarNodoId(realizadas, id); //buscado en realizados
        
        if (resultadoIdPendientes) //solo entra si != null
        {
            printf("Tarea con ID %d encontrada en la lista de pendientes.\n", id);
            mostrarTarea(resultadoIdPendientes->T);

        }else if (resultadoIdRealizadas)
        {
            printf("Tarea con ID %d encontrada en la lista de realizadas.\n", id);
            mostrarTarea(resultadoIdRealizadas->T);
        }else
        {
            printf("No se encontro una tarea con ID %d en ninguna lista.\n", id);
        }
        break;

    case 2:
        printf("Ingrese la palabra: \n");
        scanf("%s", palabra); //gets(palabra)

        Nodo *resultadoPalabraPendientes = buscarNodoPalabra(pendientes, palabra); 
        Nodo *resultadoPalabraRealizadas = buscarNodoPalabra(realizadas, palabra);

        if (resultadoPalabraPendientes) //solo entra si != null
        {
            printf("Tarea con palabra clave '%s' encontrada en la lista de pendientes.\n", palabra);
            mostrarTarea(resultadoPalabraPendientes->T);

        }else if (resultadoPalabraRealizadas)
        {
            printf("Tarea con palabra clave '%s' encontrada en la lista de realizadas.\n", palabra);
            mostrarTarea(resultadoPalabraRealizadas->T);
        }else
        {
            printf("No se encontro una tarea con la palabra %d en ninguna lista.\n", id);
        }
        break;
    
    default: printf("Opcion NO valida \n");
        break;
    }
}

void liberarLista(Nodo *lista) {
    Nodo *temp;
    while (lista != NULL) {
        temp = lista;
        lista = lista->Siguiente;
        free(temp->T.Descripcion); // Liberar memoria de la descripción
        free(temp); // Liberar memoria del nodo
    }
}

