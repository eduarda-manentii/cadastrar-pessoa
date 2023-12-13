#pragma once

#include <gtkmm.h>

struct EntryBox : public Gtk::HBox {
    Gtk::Label label;
    Gtk::Entry entry;

    EntryBox(const Glib::ustring& name);
};
