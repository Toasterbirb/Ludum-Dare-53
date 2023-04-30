#include "Frames.hpp"
#include <unordered_map>
#include <vector>

using namespace Birb;

void LoadFrames(std::unordered_map<std::string, Frame>& frame_map)
{
	std::vector<Frame> frames_to_load;

	/* Inside of the tent */
	Frame inside_tent("tent_inside.jpg");

	ClickTarget go_outside_of_tent(Rect(565, 140, 410, 587), "tent_outside.jpg");
	inside_tent.click_targets.push_back(go_outside_of_tent);

	DialogueEntry tent_start_dialogue;
	tent_start_dialogue.dialogue.push_back("Awwh... My head hurts so bad...");
	tent_start_dialogue.dialogue.push_back("My neighbor seems to be having a party again");
	tent_start_dialogue.dialogue.push_back("I need to figure out something permanent...\nTelling him to breathe and blink manually doesn't\nseem to be enough anymore");
	inside_tent.dialogue_text.push_back(tent_start_dialogue);

	DialogueEntry tent_rope_dialogue;
	tent_rope_dialogue.step_incrementing_dialogue = true;
	tent_rope_dialogue.required_progress = 3;
	tent_rope_dialogue.dialogue.push_back("Looks like I forgot some rope here");
	tent_rope_dialogue.dialogue.push_back("This is a bit much, but it will do after\nsome trimming");
	tent_rope_dialogue.dialogue.push_back("I should go back to the shed and finish this project");
	inside_tent.dialogue_text.push_back(tent_rope_dialogue);

	frames_to_load.push_back(inside_tent);


	/* Outside of the tent right in front of it */
	Frame outside_tent("tent_outside.jpg");

	ClickTarget go_to_shed_front_lawn(Rect(600, 64, 342, 359), "shed_front_lawn.jpg");
	outside_tent.click_targets.push_back(go_to_shed_front_lawn);

	ClickTarget go_back_to_tent_inside(Rect(1025, 322, 373, 397), "tent_inside.jpg");
	outside_tent.click_targets.push_back(go_back_to_tent_inside);

	DialogueEntry tent_outside_dialogue;
	tent_outside_dialogue.dialogue.push_back("Looks like the sun is rising already");
	tent_outside_dialogue.dialogue.push_back("Or maybe its just my neighbors disco ball doing\nits intended job");
	tent_outside_dialogue.dialogue.push_back("Maybe I should check up on my shed.\nI have plenty of materials there for dealing with\nsituations like these");
	outside_tent.dialogue_text.push_back(tent_outside_dialogue);

	frames_to_load.push_back(outside_tent);


	/* Front lawn of the shed */
	Frame shed_front_lawn("shed_front_lawn.jpg");

	ClickTarget go_back_to_tent_front(Rect(1366, 232, 226, 592), "tent_outside.jpg");
	shed_front_lawn.click_targets.push_back(go_back_to_tent_front);

	ClickTarget go_to_shed_front_door_outside(Rect(1038, 281, 242, 391), "shed_front_door_outside.jpg");
	shed_front_lawn.click_targets.push_back(go_to_shed_front_door_outside);

	ClickTarget go_to_shed_gate_view(Rect(490, 446, 162, 199), "shed_gate_view.jpg");
	shed_front_lawn.click_targets.push_back(go_to_shed_gate_view);

	frames_to_load.push_back(shed_front_lawn);


	/* Shed front door (outside) */
	Frame shed_front_door_outside("shed_front_door_outside.jpg");

	ClickTarget go_back_to_shed_front_lawn(Rect(1250, 52, 338, 755), "shed_front_lawn.jpg");
	shed_front_door_outside.click_targets.push_back(go_back_to_shed_front_lawn);

	ClickTarget go_inside_shed(Rect(365, 62, 488, 736), "shed_front_door_inside.jpg");
	shed_front_door_outside.click_targets.push_back(go_inside_shed);

	frames_to_load.push_back(shed_front_door_outside);


	/* Shed front door (inside) */
	Frame shed_front_door_inside("shed_front_door_inside.jpg", true);

	ClickTarget go_back_to_shed_frontdoor_outside(Rect(4, 299, 158, 579), "shed_front_door_outside.jpg");
	shed_front_door_inside.click_targets.push_back(go_back_to_shed_frontdoor_outside);

	ClickTarget go_to_crafting_table_main_view(Rect(827, 352, 722, 488), "shed_crafting_table_main_view.jpg");
	shed_front_door_inside.click_targets.push_back(go_to_crafting_table_main_view);

	ClickTarget go_to_pipe_collection(Rect(166, 569, 282, 196), "shed_pipe_collection.jpg");
	shed_front_door_inside.click_targets.push_back(go_to_pipe_collection);

	DialogueEntry shed_entry_dialogue;
	shed_entry_dialogue.dialogue.push_back("Ahh... Home sweet home");
	shed_entry_dialogue.dialogue.push_back("Lets have a look around and figure something out");
	shed_entry_dialogue.dialogue.push_back("I think we could start by looking for some pipes");
	shed_front_door_inside.dialogue_text.push_back(shed_entry_dialogue);

	frames_to_load.push_back(shed_front_door_inside);


	/* Shed crafting table main view */
	Frame shed_crafting_table_main_view("shed_crafting_table_main_view.jpg", true);

	ClickTarget go_to_gunpowder_corner(Rect(1269, 178, 329, 488), "shed_gunpowder_box_corner.jpg");
	shed_crafting_table_main_view.click_targets.push_back(go_to_gunpowder_corner);

	ClickTarget go_to_shed_frontdoor_inside_from_craftingtable(Rect(4, 5, 298, 892), "shed_front_door_inside.jpg");
	shed_crafting_table_main_view.click_targets.push_back(go_to_shed_frontdoor_inside_from_craftingtable);

	ClickTarget go_to_brass_cap_cabinet(Rect(452, 497, 175, 177), "shed_crafting_table_brass_cap_cabinet.jpg");
	shed_crafting_table_main_view.click_targets.push_back(go_to_brass_cap_cabinet);

	frames_to_load.push_back(shed_crafting_table_main_view);


	/* Shed crafting table brass cap cabinet */
	Frame shed_crafting_table_brass_cap_cabinet("shed_crafting_table_brass_cap_cabinet.jpg");

	ClickTarget go_back_to_shed_crafting_table_from_brass_caps(Rect(1, 1, 352, 897), "shed_crafting_table_main_view.jpg");
	shed_crafting_table_brass_cap_cabinet.click_targets.push_back(go_back_to_shed_crafting_table_from_brass_caps);

	DialogueEntry brass_cabinet_dialogue;
	brass_cabinet_dialogue.step_incrementing_dialogue = true;
	brass_cabinet_dialogue.required_progress = 2;
	brass_cabinet_dialogue.dialogue.push_back("These brass caps will do");
	brass_cabinet_dialogue.dialogue.push_back("There seems to be some even with a small hole in them");
	brass_cabinet_dialogue.dialogue.push_back("That will be useful later on");
	brass_cabinet_dialogue.dialogue.push_back("Now, lets look for some rope");
	shed_crafting_table_brass_cap_cabinet.dialogue_text.push_back(brass_cabinet_dialogue);

	frames_to_load.push_back(shed_crafting_table_brass_cap_cabinet);


	/* Pipe collection */
	Frame shed_pipe_collection("shed_pipe_collection.jpg");

	ClickTarget return_to_shed_front_door_from_pipe_collection(Rect(3, 4, 294, 874), "shed_front_door_inside.jpg");
	shed_pipe_collection.click_targets.push_back(return_to_shed_front_door_from_pipe_collection);

	DialogueEntry pipe_collection_dialogue;
	pipe_collection_dialogue.step_incrementing_dialogue = true;
	pipe_collection_dialogue.dialogue.push_back("My sweet sweet pipe collection...\nI have missed you so much");
	pipe_collection_dialogue.dialogue.push_back("I think one will be enough for this case.");
	pipe_collection_dialogue.dialogue.push_back("Next, we'll need some gunpowder to fill this\npipe with");
	shed_pipe_collection.dialogue_text.push_back(pipe_collection_dialogue);

	frames_to_load.push_back(shed_pipe_collection);


	/* Gunpowder corner */
	Frame shed_gunpowder_box_corner("shed_gunpowder_box_corner.jpg", true, {0, 0, 1, 1, 1});

	ClickTarget return_to_shed_crafting_table_from_gunpowder(Rect(8, 8, 330, 884), "shed_crafting_table_main_view.jpg");
	shed_gunpowder_box_corner.click_targets.push_back(return_to_shed_crafting_table_from_gunpowder);

	DialogueEntry gunpowder_dialogue;
	gunpowder_dialogue.step_incrementing_dialogue = true;
	gunpowder_dialogue.required_progress = 1;
	gunpowder_dialogue.dialogue.push_back("There you are!\nI thought I had already used up everything...");
	gunpowder_dialogue.dialogue.push_back("Next we'll need some caps to close up the pipe with");
	shed_gunpowder_box_corner.dialogue_text.push_back(gunpowder_dialogue);

	frames_to_load.push_back(shed_gunpowder_box_corner);


	/* Shed front lawn road gate place thingie */
	Frame shed_gate_view("shed_gate_view.jpg");

	ClickTarget go_to_first_roadstep_from_shed(Rect(545, 285, 535, 603), "first_road_step_from_shed.jpg");
	shed_gate_view.click_targets.push_back(go_to_first_roadstep_from_shed);

	frames_to_load.push_back(shed_gate_view);


	/* First road step from shed */
	Frame first_road_step_from_shed("first_road_step_from_shed.jpg");

	ClickTarget return_back_to_shed_from_first_roadstep_1(Rect(4, 139, 299, 758), "look_back_at_shed_from_first_roadstep.jpg");
	first_road_step_from_shed.click_targets.push_back(return_back_to_shed_from_first_roadstep_1);

	ClickTarget return_back_to_shed_from_first_roadstep_2(Rect(1268, 167, 328, 725), "look_back_at_shed_from_first_roadstep.jpg");
	first_road_step_from_shed.click_targets.push_back(return_back_to_shed_from_first_roadstep_2);

	ClickTarget go_to_second_road_step_from_shed(Rect(422, 310, 725, 417), "second_road_step_from_shed.jpg");
	first_road_step_from_shed.click_targets.push_back(go_to_second_road_step_from_shed);

	frames_to_load.push_back(first_road_step_from_shed);


	/* Second road step from the shed */
	Frame second_road_step_from_shed("second_road_step_from_shed.jpg");

	ClickTarget go_to_third_road_step_from_shed(Rect(426, 301, 657, 389), "third_road_step_from_shed.jpg");
	second_road_step_from_shed.click_targets.push_back(go_to_third_road_step_from_shed);

	frames_to_load.push_back(second_road_step_from_shed);


	/* Third road step from the shed */
	Frame third_road_step_from_shed("third_road_step_from_shed.jpg");

	ClickTarget go_to_empty_mailbox_closeup(Rect(507, 279, 448, 450), "mailbox_closeup_empty.jpg");
	third_road_step_from_shed.click_targets.push_back(go_to_empty_mailbox_closeup);

	ClickTarget start_returning_back_to_shed(Rect(13, 66, 295, 819), "first_return_from_neighbor.jpg");
	third_road_step_from_shed.click_targets.push_back(start_returning_back_to_shed);

	frames_to_load.push_back(third_road_step_from_shed);


	/* Empty mailbox closeup */
	Frame empty_mailbox_closeup("mailbox_closeup_empty.jpg");

	ClickTarget return_back_from_mailbox_closeup(Rect(9, 19, 346, 862), "third_road_step_from_shed.jpg");
	empty_mailbox_closeup.click_targets.push_back(return_back_from_mailbox_closeup);

	frames_to_load.push_back(empty_mailbox_closeup);


	/* Look back at the shed from first road step */
	Frame look_back_at_shed_from_first_roadstep("look_back_at_shed_from_first_roadstep.jpg");

	ClickTarget return_back_to_shed_front_lawn_from_road(Rect(466, 185, 661, 679), "shed_front_lawn.jpg");
	look_back_at_shed_from_first_roadstep.click_targets.push_back(return_back_to_shed_front_lawn_from_road);

	frames_to_load.push_back(look_back_at_shed_from_first_roadstep);


	/* First step when returning back to the shed from neighbor */
	Frame first_return_from_neighbor("first_return_from_neighbor.jpg");

	ClickTarget return_back_to_shed_2(Rect(694, 393, 571, 340), "second_return_from_neighbor.jpg");
	first_return_from_neighbor.click_targets.push_back(return_back_to_shed_2);

	frames_to_load.push_back(first_return_from_neighbor);


	/* Second step from neighbor back to the shed */
	Frame second_return_from_neighbor("second_return_from_neighbor.jpg");

	ClickTarget return_to_shed_gate_from_neighbor(Rect(588, 374, 317, 252), "look_back_at_shed_from_first_roadstep.jpg");
	second_return_from_neighbor.click_targets.push_back(return_to_shed_gate_from_neighbor);

	frames_to_load.push_back(second_return_from_neighbor);


	for (size_t i = 0; i < frames_to_load.size(); ++i)
	{
		frame_map[frames_to_load[i].name] = frames_to_load[i];
	}
}
