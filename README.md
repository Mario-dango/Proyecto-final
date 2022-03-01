# Proyecto integrador de materia "Informática"

En el presente proyecto se encuentra el desarrollo concretado de una abstracción de central meteorológica montada en una RaspBerry Pi 3B+ la cual toma diversos
parametros,tales cómo humedad relativa y temperatura además de ello, la misma posee una alertas luminicas indicadas con leds y la posibilidad de poder interactuar con la misma mediante botones pulsadores.

El programa debe de ser capaz de tomar dichos parametros de los diversos dispositivos (LM35 y DHT11) interpretarlos para dar las determinadas alertas correspondientes y a su vez los almacena en archivos tomando registro de cada vez que se ha inicializado la central, salvando los datos de inicializaciones previas. Además de dar la posibilidad de su inicialización e interacción mediante los pulsadores que posee.

El proyecto se realizo mediante comunicación SSH para poder desarollar el programa correspondiente y todo en leguaje C implimentando librerias de terceros para la correcta interpretación de los datos.
