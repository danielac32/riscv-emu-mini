


extern	unsigned int disable(void);
extern	void	enable(void);
extern	void	restore(unsigned int);
extern void enable_interrupt_status();
extern void enable_interrupt(unsigned int);
extern void disable_interrupt_status();
extern void disable_interrupt(unsigned int);
extern void clear_interrupts();
extern void soft_trap();
extern void clkhandler(void);