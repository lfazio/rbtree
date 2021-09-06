O ?= build
LIB ?= lib

ifeq ($(V),1)
Q :=
else
Q := @
endif

ifeq ($(LTO),1)
LTO :=
else
LTO := -flto
endif

ifeq ($(DEBUG),1)
OPTIM := -Og
else
OPTIM := -Os
endif

all: $(O)/librbtree.a

$(O)/librbtree.a: $(O)/rbtree.o
	$(Q)echo "[LIB] librbtree.a"
	$(Q)ar -rcs $@ $<

$(O)/%.o: %.c %.h
	$(Q)mkdir -p $(O)
	$(Q)echo "[CC] rbtree.o"
	$(Q)$(CC) -c $(OPTIM) $(LTO) -I. -o $@ $<

install: all
	$(Q)install -m 755 -d $(LIB)
	$(Q)install -m 664 -p -D $(O)/librbtree.a -t $(LIB)

check: install
	$(Q)$(MAKE) -C tests

clean:
	$(Q)echo "[CLEAN] Intermediate items..."
	$(Q)rm -f $(O)/rbtree.o
	@find -name "*~" -exec rm -f {} \;

cleanall: clean
	$(Q)echo "[CLEAN] Final items..."
	$(Q)rm -f $(O)/librbtree.a
	$(Q)rm -rf $(O)

distclean: cleanall
	$(Q)echo "[CLEAN] Installed items..."
	$(Q)rm -rf $(LIB)
