#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>

void alert_cb(Fl_Widget *, void *) {
  fl_alert("alert");
}

void exit_cb(Fl_Widget *, void *) {
  exit(0);
}

int main(int argc, char **argv) {
  Fl_Window *window = new Fl_Window(300, 180);
  Fl_Button *alert_button = new Fl_Button(100, 60, 100, 20, "alert");
  Fl_Button *exit_button = new Fl_Button(100, 100, 100, 20, "exit");

  alert_button->callback((Fl_Callback *)alert_cb);
  exit_button->callback((Fl_Callback *)exit_cb);

  window->end();
  window->show(argc, argv);
  return Fl::run();
}
