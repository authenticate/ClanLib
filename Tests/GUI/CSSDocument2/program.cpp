
#include "precomp.h"
#include "program.h"
#include <ClanLib/application.h>

using namespace clan;

Application clanapp(&Program::main);

int Program::main(const std::vector<std::string> &args)
{
	SetupCore setup_core;
	SetupDisplay setup_display;
	SetupGL setup_gl;
	SetupGUI setup_gui;

	GUIManager gui(".");
	gui.add_resources("../../../Resources/GUIThemeLuna/resources.xml");

	GUITopLevelDescription window_desc;
	window_desc.set_title("GUI Test");
	window_desc.set_size(Size(1024, 768), false);
	window_desc.set_allow_resize(true);

	GUIComponent *root = new GUIComponent(&gui, window_desc);
	root->set_tag_name("root");
	root->func_close().set(&Program::on_close, root);
	GUIComponent *child1 = new GUIComponent(root);
	child1->set_tag_name("child");
	child1->set_id("child1");
	GUIComponent *child11 = new GUIComponent(child1);
	child11->set_tag_name("child");
	child11->set_id("child11");
	GUIComponent *child2 = new GUIComponent(root);
	child2->set_tag_name("child");
	child2->set_id("child2");
	GUIComponent *child22 = new GUIComponent(child2);
	child22->set_tag_name("child");
	child22->set_id("child22");
	GUIComponent *child23 = new GUIComponent(child2);
	child23->set_tag_name("child");
	child23->set_id("child23");

	gui.exec();

	return 0;
}

bool Program::on_close(GUIComponent *component)
{
	component->exit_with_code(0);
	return true;
}