#include "entry_box.h"

EntryBox::EntryBox(const Glib::ustring& name) : label(name) {
    add(label);
    add(entry);
}
