/*
 * dispatcher.h
 *
 *  Created on: Jul 12, 2024
 *      Author: eyalk
 */

#ifndef INC_DISPATCHER_H_
#define INC_DISPATCHER_H_

#include "defs.h"
#include "stdio.h"
#include "main.h"
#include "string.h"

void vDispatcherCode(void *pvParameters);
void generateDispatcherMSG(DispatcherPacket* hDispPacket);



#define SHORTEST_TASK_DURATION_IN_TICKS		pdMS_TO_TICKS(10)
#define LONGEST_TASK_DURATION_IN_TICKS		pdMS_TO_TICKS(300)

/**************** Random Dispatcher Messages ****************/
#define AMB_STRINGS_LEN 		8
#define POLICE_STRINGS_LEN 		5
#define FIRE_STRINGS_LEN 		9
#define CORONA_STRINGS_LEN 		8



// Ambulance Messages
#define AMBstr1		"I suffered a severe paper cut! Send an Ambulance ASAP!"
#define AMBstr2		"I don't want to go to school today, told my mom I was sick and I need your cooperation!"
#define AMBstr3		"I have a headache!"
#define AMBstr4		"I'm hung over"
#define AMBstr5		"I burnt my tongue drinking a coffee which was too hot!"
#define AMBstr6  	"I stubbed my toe on the coffee table! This is an emergency!"
#define AMBstr7  	"I sneezed three times in a row! What's happening to me?!"
#define AMBstr8  	"My pet hamster looks at me funny. Am I okay?"

// Police Messages
#define POLstr1		"Help! My stamp collection was stolen!"
#define POLstr2		"Help! My dog ate my homework!"
#define POLstr3		"Help! My neighbor is playing The Beatles non-stop!"
#define POLstr4  	"Help! Someone parked in my favorite spot!"
#define POLstr5  	"Help! My WiFi is down and I suspect foul play!"

// Fire Dep Messages
#define FIREstr1 	"Help! My cat is stuck on a tree and can't get down!"
#define FIREstr2 	"Help! My scooter's LiPO is on fire!"
#define FIREstr3 	"Help! There's a strong scent of gas in my house"
#define FIREstr4 	"Help! I'm pretty good looking most days, but today I'm on FIRE!"
#define FIREstr5 	"Help! My microwave caught fire while making popcorn!"
#define FIREstr6 	"Help! My candles have set the mood too much!"
#define FIREstr7 	"Help! My dinner is burnt and now my kitchen is too!"
#define FIREstr8 	"Help! I tried to cook and now my stove is rebelling!"
#define FIREstr9 	"Help! My BBQ party got a little too lit!"

// Corona Messages
#define CORstr1		"I need someone to test me for COVID"
#define CORstr2		"I ate a bat, do I have COVID?"
#define CORstr3		"I'm Batman, do I have COVID?"
#define CORstr4 	"I coughed in a Zoom meeting, am I infected?"
#define CORstr5 	"I binge-watched pandemic movies, do I need a test?"
#define CORstr6 	"My dog sneezed, should we both quarantine?"
#define CORstr7 	"I ran out of hand sanitizer, is that a symptom?"
#define CORstr8 	"I touched my face, now what?"



#endif /* INC_DISPATCHER_H_ */
