#include <stdlib.h>
#include <stdbool.h>

int main() {
    typedef struct Regs {
        uint16_t    rax, rbx;   //Primary Accumulator, Base Register
        uint16_t    rcx, rdx;   //Counter Register, Data Register
    } regs_t;

    typedef struct Flags {
        bool        IF, IR, HF;
        bool        CF, ZF;
        bool        EF, GF, LF;
    } flags_t;

    typedef struct CPU {
        //CPU Values
        uint16_t    ram[V16_RAMSIZE];   //Random Access Memory
        uint16_t    iop[V16_IOPSIZE];   //Input-Output Ports
        uint16_t    idt[V16_IDTSIZE];   //Interrupt vectors table (Interrupt Description Table)
        flags_t     flags;              //Flags
        regs_t      reg_m, reg_a;       //Main and Alt register files
        regs_t *    reg_current;        //Current register file
        uint16_t    rip, rsp, rex;      //Internal Registers: Instruction Pointer, Stack Pointer, EXtended Accumulator

        //Emulator values
        bool        reg_swapped;        //Is current register file alt
        bool        running;            //Is cpu running
        uint32_t    cycles;             //RAM access counter
    } cpu_t;

    typedef struct Opd {
        uint8_t     code : 4;
        uint16_t    value;
        uint16_t    nextw;
    } opd_t;

    cpu_t * cpu_create(void);                    //Создаем экземпляр процессора
    void cpu_delete(cpu_t *);                    //Удаляем экземпляр процессора
    void cpu_load(cpu_t *, const char *);        //Загружаем ROM в память
    void cpu_rswap(cpu_t *);                     //Меняем наборы регистров
    uint16_t cpu_nextw(cpu_t *);                 //RAM[RIP++]. Nuff said
    void cpu_getop(cpu_t *, opd_t *, uint8_t);   //Читаем операнд
    void cpu_setop(cpu_t *, opd_t *, uint16_t);  //Пишем операнд
    void cpu_tick(cpu_t *);                      //Выполняем одну инструкцию
    void cpu_loop(cpu_t *);                      //Выполняем инструкции, пока процессор работает

    void cpu_tick(cpu_t *cpu)
    {
        //Если была выполнена инструкция HLT, то функция ничего не сделает
        if(cpu->flags.HF) {
        //Если к тому же обнулен флаг прерываний, то паузу снимать уже нечему
        if(!cpu->flags.IF) {
            cpu->running = false;
        }

        return;
    }

    //Получаем следующее слово и декодируем как инструкцию
    uint16_t nw = cpu_nextw(cpu);
    uint8_t op = ((nw >> 8) & 0xFF);
    uint8_t ob = ((nw >> 4) & 0x0F);
    uint8_t oa = ((nw >> 0) & 0x0F);    //А потому что дизайн кода

    //Создаем структуры операндов
    opd_t opdB = { 0 };
    opd_t opdA = { 0 };

    //И читаем их значения
    cpu_getop(cpu, &opdB, ob);
    cpu_getop(cpu, &opdA, oa);

    //Дальше для сокращения и улучшения читабельности кода делаем переменные-значения операндов
    uint16_t B = opdB.value;
    uint16_t A = opdA.value;
    uint32_t R = 0xFFFFFFFF;    //Один очень интересный костыль
    bool clearf = true;         //Будут ли флаги условий чиститься после выполнения инструкции?

    //И начинаем творить магию!
    switch(op) {
        //Здесь мы проходим все возможные опкоды. Те, которые пишут результаты, меняют значение переменной R
    }

    //Чистим флаги условий
    if(clearf) {
        cpu->flags.EF = false;
        cpu->flags.GF = false;
        cpu->flags.LF = false;
    }

    //Очень интересный костыль, максимальное 32-битное значение при 16-битных операциях
    //  равно 0xFFFF0000, то есть 0xFFFF << 16
    //  А поэтому очень удобно для результата использовать 32-битное число
    if(R != 0xFFFFFFFF) {
        cpu_setop(cpu, &opdB, (R & 0xFFFF));
        cpu->rex = ((R >> 16) & 0xFFFF);
        cpu->flags.CF = (cpu->rex != 0);
        cpu->flags.ZF = (R == 0);
   }

   return;
}
