**City Dispatcher Project**

The City Dispatcher project is implemented on the STM32F746ZG-NUCLEO board.
I created this project in order to gain some experience using the FreeRTOS OS
and understand how its various features work and communicate with each other.

**Project Description**

The City Dispatcher project simulates the operation control room, responsible
for the management of receiving emergency/urgnet calls and forwarding them to the 
relevant handler. 
The project utilizes the use of FreeRTOS's task management features, queues, inter-task communication methods,
synchronization processes(mutexes/semaphores), timers, etc...

**How It Works?**

Every handler type(Ambulances, Police, Fire Department, Corona treatments) have a limited amount of tasks attached to it.
Each task can perform a single call at any given time. When a task has been handled, it will wait(in BLOCKED state) for
a task manager task to allocate it with a new call to handle.

The amount of active tasks(in RUNNING state) of all the departments combined, at any given time, is limited, and is configurable.

When a task finishes handling a call, it will print to the terminal through UART.
Pressing the button on the STM32F746ZG-NUCLEO board will display various data regarding the dispatcher's operation,
as well it will suspend the operation of the task management task, and the printing of tasks prints until another button press
occurs.


**Connecting Through UART**
UART connection is done at a baud rate of 115200 bits/s, and through the board's COM# connection.

Pressing the USER button will display details regarding the program's operation:
<img width="836" alt="image" src="https://github.com/user-attachments/assets/e66bc6fe-83e9-4166-97bb-370e199f4ed5">

At the start of the program, all tasks are created sequentially:
<img width="851" alt="image" src="https://github.com/user-attachments/assets/ead6a4ec-2e10-4250-9514-e84f962f8e28">


During normal operations, handled tasks are printed. 
<img width="1023" alt="image" src="https://github.com/user-attachments/assets/b8c6a8b7-3f97-477a-8454-b2fa3ffe6007">




