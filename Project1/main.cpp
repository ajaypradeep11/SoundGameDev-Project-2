//Guess the beat 
//3 streaming sounds. Search for "_bgm" will show three places of continuous sounds used
//8 sampled sounds. Search for "_sampledsound" will show eight places where sampled sounds are used
//ability to pause and play. Search for "_pauseplay" will show place where it is defined
//5 sounds where attributes are changed. Search for "_attrib" will show places where they are defined
//Loaded information from an external file. Search for "_file" will show places where they are defined
//This a guessing game where people can guess the played beat using options.
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>

//To use Sleep function included <Windows.h>
#include <Windows.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <FMOD/fmod.hpp>

#include <rssgl/GLText.h>
#include <fstream>
#include <vector>

GLuint _width = 1920;
GLuint _height = 1080;

GLFWwindow* _window = NULL;
std::string __applicationName = "Guess the beat";

FMOD::System* _system = NULL;
FMOD_RESULT _result = FMOD_OK;

//Sound variable for Background music
FMOD::Sound* _soundStartBgm = NULL;
FMOD::Sound* _soundMidBgm = NULL;
FMOD::Sound* _soundEndBgm = NULL;

//Sound variable for Game won or lose
FMOD::Sound* _soundGameWon = NULL;
FMOD::Sound* _soundGameLost = NULL;

//Sound variable for automated question sound
FMOD::Sound* _soundQuestion1 = NULL;
FMOD::Sound* _soundQuestion2 = NULL;
FMOD::Sound* _soundQuestion3 = NULL;

//Sound variable for click sound when clicking options
FMOD::Sound* _soundClick = NULL;

//LEVEL 1
//Sound variable for LEVEL 1 questions options
FMOD::Sound* _soundQuestionSound = NULL;
FMOD::Sound* _soundQuestion1Sound = NULL;
FMOD::Sound* _soundQuestion2Sound = NULL;
FMOD::Sound* _soundQuestion3Sound = NULL;

//LEVEL 2
//Sound variable for LEVEL 2 question sound
FMOD::Sound* _soundLevel2Sound = NULL;

//Sound variable for automated sound LEVEL 2 answer
FMOD::Sound* _soundLevel2Left = NULL;
FMOD::Sound* _soundLevel2Center = NULL;
FMOD::Sound* _soundLevel2Right = NULL;

//LEVEL 3
//Sound variable for automated sound LEVEL 3 question and answer
FMOD::Sound* _soundLevel3Sound = NULL;

//Channel for sounds
FMOD::Channel* _channel = NULL;
FMOD::Channel* _channelStartBgm = NULL;
FMOD::Channel* _channelQuestion = NULL;
FMOD::Channel* _channelQuestion2 = NULL;
FMOD::Channel* _channelQuestion3 = NULL;


FMOD::ChannelGroup* _channelGroupBGM = nullptr;

FMOD::ChannelGroup* _channelGroupVoice = nullptr;

FMOD::ChannelGroup* _channelGroupOption = nullptr;

FMOD::ChannelGroup* _channelGroupSong = nullptr;

FMOD::ChannelGroup* _channelGroupMaster = nullptr;

FMOD::ChannelGroup* _selectedChannelGroup = nullptr;

RSS::GLText* _text;
//Rendering same row again and again
GLuint _textRowIndex = 2;

//Boolean values to check condition
bool _isPaused = false;
bool _isPlaying = false;
bool _isGameEntered = false;
bool isStartgame = false;
bool isEndgame = false;
bool isLevel1 = false;
bool isLevel2 = false;
bool isLevel3 = false;

bool zKey = false;
bool xKey = false;
bool cKey = false;
bool vKey = false;

//To check whether the max attempts is 3. if greater than 3, then gameover.
static int count = 0;

//To get audio from filelist
std::vector<std::string> _audioList;
std::vector<std::string> _audioBGM;
std::vector<std::string> _audioQuestionLevel1;
std::vector<std::string> _audioQuestionLevel2and3;
std::vector<std::string> _audioAutomatedSound;
std::vector<std::string> _audioGameWonOrLose;

#define BUFFER_SIZE 255
char _buffer[BUFFER_SIZE];
FMOD::DSP* _dspEcho = nullptr;
FMOD::DSP* _dspFlange = nullptr;
FMOD::DSP* _dspDistortion = nullptr;
FMOD::DSP* _dspChorus = nullptr;
FMOD::DSP* _dspDelay = nullptr;
FMOD::DSP* _dspTremolo = nullptr;
FMOD::DSP* _dspPan = nullptr;
FMOD::DSP* _dspConvolutionreverb = nullptr;
FMOD::DSP* _dspObjectpan = nullptr;
FMOD::DSP* _dspOscillator = nullptr;
FMOD::DSP* _dspFader = nullptr;
FMOD::DSP* _dspLowpass = nullptr;
FMOD::DSP* _dspHighpass = nullptr;
FMOD::DSP* _dspNormalize = nullptr;
FMOD::DSP* _dspLimiter = nullptr;
FMOD::DSP* _dspCompressor = nullptr;

//Used to change background color
float red = 0.0f;
float blue = 0.0f;
float green = 0.0f;


//Function used to replay the options for LEVEL 1
void callOption() {
	if (!_isPlaying) {
		//_sampledsound beats for Level 1
		_result = _system->playSound(_soundQuestionSound, _channelGroupOption, false, &_channelQuestion);
		Sleep(1000);
		//_sampledsound beats for Level 1
		_result = _system->playSound(_soundQuestion1Sound, _channelGroupOption, false, &_channelQuestion);
		Sleep(1000);
		//_sampledsound beats for Level 1
		_result = _system->playSound(_soundQuestion2Sound, _channelGroupOption, false, &_channelQuestion);
		Sleep(1000);
		//_sampledsound beats for Level 1
		_result = _system->playSound(_soundQuestion3Sound, _channelGroupOption, false, &_channelQuestion);
		if (_result != FMOD_OK) {
			fprintf(stderr, "unable to play sound");
		}
	}
}

void callOptionLvl2() {
	if (!_isPlaying) {
		//_sampledsound for left side voice
		_result = _system->playSound(_soundLevel2Left, _channelGroupOption, false, &_channelQuestion);
		Sleep(1000);
		//_sampledsound for center voice
		_result = _system->playSound(_soundLevel2Center, _channelGroupOption, false, &_channelQuestion);
		Sleep(1000);
		//_sampledsound for right side voice
		_result = _system->playSound(_soundLevel2Right, _channelGroupOption, false, &_channelQuestion);
		if (_result != FMOD_OK) {
			fprintf(stderr, "unable to play sound");
		}
	}
}

void callOptionLvl3() {
	if (!_isPlaying) {
		//_sampledsound beats for Level 3
		_result = _system->playSound(_soundLevel3Sound, _channelGroupOption, false, &_channelQuestion3);
		//_attrib Reverb is changed
		_channelQuestion3->setReverbProperties(4, 1.0f);

		Sleep(1000);

		_result = _system->playSound(_soundLevel3Sound, _channelGroupOption, false, &_channelQuestion3);
		//_attrib LowpassGain is changed
		_channelQuestion3->setLowPassGain(1.0f);
		Sleep(1000);


		_result = _system->playSound(_soundLevel3Sound, _channelGroupOption, false, &_channelQuestion3);
		_channelQuestion3->setPitch(2.0);

		if (_result != FMOD_OK) {
			fprintf(stderr, "unable to play sound");
		}
	}
}

void changeBackground(float r, float g, float b) {
	red = r;
	green = g;
	blue = b;
}

void incrementAndCheck() {

	if (isStartgame)
	{
		_channelQuestion->isPlaying(&_isPlaying);
	}
	else if (isLevel1)
	{
		_channelQuestion2->isPlaying(&_isPlaying);
	}
	else if (isLevel2)
	{
		_channelQuestion3->isPlaying(&_isPlaying);
	}

	if (!_isPlaying) {
		//_sampledsound click sound when option are clicked
		_result = _system->playSound(_soundClick, 0, false, &_channelQuestion);
		if (_result != FMOD_OK) {
			fprintf(stderr, "unable to play sound");
		}
		count++;
		if (count >= 3) {
			isEndgame = true;
			isStartgame = false;
			isLevel1 = false;
			isLevel2 = false;
			changeBackground(0.341f, 0.0f, 0.023f);
			count = 0;
			_channelStartBgm->stop();

			//_bgm endbgm
			_result = _system->playSound(_soundEndBgm, _channelGroupBGM, false, &_channelStartBgm);
			//_attrib volume attribute is changed
			_channelStartBgm->setVolume(0.09);
			//_sampledsound beats for game lose
			_result = _system->playSound(_soundGameLost, _channelGroupSong, false, &_channelQuestion);
			if (_result != FMOD_OK) {
				fprintf(stderr, "unable to play sound");
			}
		}
	}

}

//Keypress callback
static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	float volume = 0.0f;
	float pitch = 0.0f;
	bool isPaused = false;
	float stepSize = 0.02f;
	float volumeMax = 1.0f;
	float volumeMin = 0.0f;
	float pitchMax = 2.0f;
	float pitchMin = 0.0f;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {

		//When game is entered, this snippet is called
		if (!_isGameEntered) {
			changeBackground(0.15f, 0.15f, 0.15f);
			_isGameEntered = true;
			isStartgame = true;
			_channelStartBgm->stop();

			//_bgm midbgm
			//_soundMidBgm is called as that is the bgm for level1
			_result = _system->playSound(_soundMidBgm, _channelGroupBGM, false, &_channelStartBgm);
			_channelStartBgm->setVolume(0.09);
			if (_result != FMOD_OK) {
				fprintf(stderr, "unable to play sound");
			}

			//_soundQuestion1 is called as that is the automated voice for level1 question
			_result = _system->playSound(_soundQuestion1, _channelGroupVoice, false, &_channelQuestion);
			if (_result != FMOD_OK) {
				fprintf(stderr, "unable to play sound");
			}
		}
	}

	// LEVEL 1,2 and 3
	//Q press to hear question
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		//we check isStartgame,isLevel1 and isLevel2 to check whether user is in LEVEL 1, 2 or 3 respectively
		if (isStartgame) {
			_channelQuestion->isPlaying(&_isPlaying);
			if (!_isPlaying) {

				_result = _system->playSound(_soundQuestion1Sound, _channelGroupOption, false, &_channelQuestion);
				if (_result != FMOD_OK) {
					fprintf(stderr, "unable to play sound");
				}
			}
		}
		else if (isLevel1) {
			_channelQuestion2->isPlaying(&_isPlaying);
			if (!_isPlaying) {
				_result = _system->playSound(_soundLevel2Sound, _channelGroupOption, false, &_channelQuestion2);
				//_attrib balance is changed. Left/Right
				_channelQuestion2->setPan(1);
				if (_result != FMOD_OK) {
					fprintf(stderr, "unable to play sound");
				}
			}
		}
		else if (isLevel2) {
			_channelQuestion3->isPlaying(&_isPlaying);
			if (!_isPlaying) {
				_result = _system->playSound(_soundLevel3Sound, _channelGroupOption, false, &_channelQuestion3);
				//_attrib Pitch is changed
				_channelQuestion3->setPitch(2.0);
				if (_result != FMOD_OK) {
					fprintf(stderr, "unable to play sound");
				}
			}
		}
	}

	//R press to hear options DEPENDS ON LEVEL
	else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		if (isStartgame) {
			_channelQuestion->isPlaying(&_isPlaying);
			callOption();
		}
		else if (isLevel1) {
			_channelQuestion2->isPlaying(&_isPlaying);
			callOptionLvl2();
		}
		else if (isLevel2) {
			_channelQuestion3->isPlaying(&_isPlaying);
			callOptionLvl3();
		}


	}

	//pause background music we have three bgm. Initial bgm, game start bgm and game lost bgm.
	else if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
		//_pauseplay pause and play used
		if (_channelStartBgm) {
			_result = _channelStartBgm->getPaused(&_isPaused);
			_result = _channelStartBgm->setPaused(!_isPaused);
			_result = _channelStartBgm->getPaused(&_isPaused);
		}
	}

	//Level 1,2 and 3 answer options
	else if (((key == GLFW_KEY_1) || (key == GLFW_KEY_KP_1)) && action == GLFW_PRESS) {
		if (isStartgame) {
			incrementAndCheck();
		}
		else if (isLevel1) {
			incrementAndCheck();
		}
		else if (isLevel2) {
			incrementAndCheck();
		}
	}
	else if (((key == GLFW_KEY_2) || (key == GLFW_KEY_KP_2)) && action == GLFW_PRESS) {
		if (isStartgame) {
			_channelQuestion->isPlaying(&_isPlaying);
			if (!_isPlaying) {
				//Level 1 passed
				isLevel1 = true;
				isStartgame = false;
				changeBackground(0.360f, 0.227f, 0.0f);
				_result = _system->playSound(_soundQuestion2, _channelGroupVoice, false, &_channelQuestion2);
				if (_result != FMOD_OK) {
					fprintf(stderr, "unable to play sound");
				}
			}
			count = 0;
		}
		else if (isLevel1) {
			incrementAndCheck();
		}
		else if (isLevel2) {
			incrementAndCheck();
		}
	}
	else if (((key == GLFW_KEY_3) || (key == GLFW_KEY_KP_3)) && action == GLFW_PRESS) {
		if (isStartgame) {
			incrementAndCheck();
		}
		else if (isLevel1) {
			_channelQuestion2->isPlaying(&_isPlaying);
			if (!_isPlaying) {
				//Level 2 passed
				isLevel2 = true;
				isLevel1 = false;
				changeBackground(0.074f, 0.0f, 0.360f);
				count = 0;
				_result = _system->playSound(_soundQuestion3, _channelGroupVoice, false, &_channelQuestion3);
				if (_result != FMOD_OK) {
					fprintf(stderr, "unable to play sound");
				}
			}
			count = 0;		
		}
		else if (isLevel2) {
			_channelQuestion3->isPlaying(&_isPlaying);
			if (!_isPlaying) {
				//Level 3 passed
				isLevel3 = true;
				isLevel2 = false;
				changeBackground(0.0f, 0.239f, 0.066f);
				count = 0;
				//_sampledsound beats for game won
				_result = _system->playSound(_soundGameWon, _channelGroupSong, false, &_channelQuestion3);
				if (_result != FMOD_OK) {
					fprintf(stderr, "unable to play sound");
				}
			}
			count = 0;
		}
	}
	else if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		_selectedChannelGroup = _channelGroupBGM;

		_result = _system->createDSPByType(FMOD_DSP_TYPE_ECHO, &_dspEcho);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to create dsp\n");

		}
		//Append dsp to channel index 0
		//_result = _vecChannels.at(0)->addDSP(0, _dspEcho);
		_result = _selectedChannelGroup->addDSP(0, _dspEcho);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to add dsp to channel\n");

		}
		//Enalbe dsp
		_result = _dspEcho->setActive(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp to active\n");

		}
		//Bypass dsp for now
		_result = _dspEcho->setBypass(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp bypass\n");

		}



		//Flange
		_result = _system->createDSPByType(FMOD_DSP_TYPE_FLANGE, &_dspFlange);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to create dsp\n");
		}
		//Append dsp to channel index 0

		_result = _selectedChannelGroup->addDSP(0, _dspFlange);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to add dsp to channel\n");
		}
		//Enalbe dsp
		_result = _dspFlange->setActive(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp to active\n");
		}
		//Bypass dsp for now
		_result = _dspFlange->setBypass(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp bypass\n");
		}


		//NINE - objectpan

		_result = _system->createDSPByType(FMOD_DSP_TYPE_OBJECTPAN, &_dspObjectpan);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to create dsp\n");
		}
		//Append dsp to channel index 0

		_result = _selectedChannelGroup->addDSP(2, _dspObjectpan);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to add dsp to channel8\n");
		}
		//Enalbe dsp
		_result = _dspObjectpan->setActive(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp to active\n");
		}
		//Bypass dsp for now
		_result = _dspObjectpan->setBypass(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp bypass\n");
		}


		//THIRTEEN - highpass

		_result = _system->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &_dspHighpass);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to create dsp\n");
		}
		//Append dsp to channel index 0

		_result = _selectedChannelGroup->addDSP(3, _dspHighpass);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to add dsp to channel4\n");
		}
		//Enalbe dsp
		_result = _dspHighpass->setActive(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp to active\n");
		}
		//Bypass dsp for now
		_result = _dspHighpass->setBypass(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp bypass\n");
		}


		zKey = true;
		xKey = false;
		cKey = false;
		vKey = false;
	}
	else if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		_selectedChannelGroup = _channelGroupVoice;

		//Distortion
		_result = _system->createDSPByType(FMOD_DSP_TYPE_DISTORTION, &_dspDistortion);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to create dsp\n");
		}

		_result = _selectedChannelGroup->addDSP(0, _dspDistortion);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to add dsp to channel14\n");
		}
		//Enalbe dsp
		_result = _dspDistortion->setActive(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp to active\n");
		}
		//Bypass dsp for now
		_result = _dspDistortion->setBypass(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp bypass\n");
		}



		_result = _system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &_dspTremolo);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to create dsp\n");
		}
		//Append dsp to channel index 0

		_result = _selectedChannelGroup->addDSP(1, _dspTremolo);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to add dsp to channel11\n");
		}
		//Enalbe dsp
		_result = _dspTremolo->setActive(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp to active\n");
		}
		//Bypass dsp for now
		_result = _dspTremolo->setBypass(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp bypass\n");
		}


		//TEN - oscillator

		_result = _system->createDSPByType(FMOD_DSP_TYPE_OSCILLATOR, &_dspOscillator);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to create dsp\n");
		}
		//Append dsp to channel index 0

		_result = _selectedChannelGroup->addDSP(2, _dspOscillator);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to add dsp to channel7\n");
		}
		//Enalbe dsp
		_result = _dspOscillator->setActive(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp to active\n");
		}
		//Bypass dsp for now
		_result = _dspOscillator->setBypass(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp bypass\n");
		}

		//FOURTEEN - normalize

		_result = _system->createDSPByType(FMOD_DSP_TYPE_NORMALIZE, &_dspNormalize);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to create dsp\n");
		}
		//Append dsp to channel index 0

		_result = _selectedChannelGroup->addDSP(3, _dspNormalize);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to add dsp to channel3\n");
		}
		//Enalbe dsp
		_result = _dspNormalize->setActive(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp to active\n");
		}
		//Bypass dsp for now
		_result = _dspNormalize->setBypass(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp bypass\n");
		}



		zKey = false;
		xKey = true;
		cKey = false;
		vKey = false;

	}
	else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		_selectedChannelGroup = _channelGroupOption;


		//Chorus
		_result = _system->createDSPByType(FMOD_DSP_TYPE_CHORUS, &_dspChorus);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to create dsp\n");
		}
		_result = _selectedChannelGroup->addDSP(0, _dspChorus);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to add dsp to channel13\n");
		}
		_result = _dspChorus->setActive(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp to active\n");
		}
		_result = _dspChorus->setBypass(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp bypass\n");
		}

		//SEVEN - pan

		_result = _system->createDSPByType(FMOD_DSP_TYPE_PAN, &_dspPan);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to create dsp\n");
		}
		//Append dsp to channel index 0

		_result = _selectedChannelGroup->addDSP(1, _dspPan);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to add dsp to channel10\n");
		}
		//Enalbe dsp
		_result = _dspPan->setActive(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp to active\n");
		}
		//Bypass dsp for now
		_result = _dspPan->setBypass(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp bypass\n");
		}


		//ELEVEN - fader

		_result = _system->createDSPByType(FMOD_DSP_TYPE_FADER, &_dspFader);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to create dsp\n");
		}
		//Append dsp to channel index 0

		_result = _selectedChannelGroup->addDSP(2, _dspFader);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to add dsp to channel6\n");
		}
		//Enalbe dsp
		_result = _dspFader->setActive(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp to active\n");
		}
		//Bypass dsp for now
		_result = _dspFader->setBypass(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp bypass\n");
		}



		//FIFTEEN - limiter

		_result = _system->createDSPByType(FMOD_DSP_TYPE_LIMITER, &_dspLimiter);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to create dsp\n");
		}
		//Append dsp to channel index 0

		_result = _selectedChannelGroup->addDSP(3, _dspLimiter);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to add dsp to channel2\n");
		}
		//Enalbe dsp
		_result = _dspLimiter->setActive(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp to active\n");
		}
		//Bypass dsp for now
		_result = _dspLimiter->setBypass(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp bypass\n");
		}

		zKey = false;
		xKey = false;
		cKey = true;
		vKey = false;

	}
	else if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		_selectedChannelGroup = _channelGroupSong;

		//Delay
		_result = _system->createDSPByType(FMOD_DSP_TYPE_DELAY, &_dspDelay);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to create dsp\n");
		}
		//Append dsp to channel index 0

		_result = _selectedChannelGroup->addDSP(1, _dspDelay);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to add dsp to channel12\n");
		}
		//Enalbe dsp
		_result = _dspDelay->setActive(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp to active\n");
		}
		//Bypass dsp for now
		_result = _dspDelay->setBypass(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp bypass\n");
		}

		//EIGHT - convolutionreverb

		_result = _system->createDSPByType(FMOD_DSP_TYPE_CONVOLUTIONREVERB, &_dspConvolutionreverb);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to create dsp\n");
		}
		//Append dsp to channel index 0

		_result = _selectedChannelGroup->addDSP(1, _dspConvolutionreverb);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to add dsp to channel9\n");
		}
		//Enalbe dsp
		_result = _dspConvolutionreverb->setActive(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp to active\n");
		}
		//Bypass dsp for now
		_result = _dspConvolutionreverb->setBypass(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp bypass\n");
		}

		//TWELVE - lowpass

		_result = _system->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &_dspLowpass);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to create dsp\n");
		}
		//Append dsp to channel index 0

		_result = _selectedChannelGroup->addDSP(2, _dspLowpass);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to add dsp to channel5\n");
		}
		//Enalbe dsp
		_result = _dspLowpass->setActive(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp to active\n");
		}
		//Bypass dsp for now
		_result = _dspLowpass->setBypass(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp bypass\n");
		}


		//SIXTEEN - compressor

		_result = _system->createDSPByType(FMOD_DSP_TYPE_COMPRESSOR, &_dspCompressor);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to create dsp\n");
		}
		//Append dsp to channel index 0

		_result = _selectedChannelGroup->addDSP(3, _dspCompressor);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable to add dsp to channel1\n");
		}
		//Enalbe dsp
		_result = _dspCompressor->setActive(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp to active\n");
		}
		//Bypass dsp for now
		_result = _dspCompressor->setBypass(true);
		if (FMOD_OK != _result) {
			fprintf(stderr, "Unable set dsp bypass\n");
		}
		zKey = false;
		xKey = false;
		cKey = false;
		vKey = true;
	}
	else if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		_selectedChannelGroup = _channelGroupMaster	;
	}
	else if (((key == GLFW_KEY_4) || (key == GLFW_KEY_KP_4)) && action == GLFW_PRESS) {
		if (isStartgame) {
			incrementAndCheck();
		}
	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		_result = _selectedChannelGroup->getPaused(&isPaused);
		if (_result != FMOD_OK) {
			fprintf(stderr, "Unable to read pause");
		}
		_result = _selectedChannelGroup->setPaused(!isPaused);
		if (_result != FMOD_OK) {
			fprintf(stderr, "Unable to set pause");
		}

	}
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		_result = _selectedChannelGroup->getVolume(&volume);
		if (_result != FMOD_OK) {
			fprintf(stderr, "Unable to read volume");
		}
		//Clamp volume
		volume = (volume >= volumeMax) ? volumeMax : (volume + stepSize);
		_result = _selectedChannelGroup->setVolume(volume);
		if (_result != FMOD_OK) {
			fprintf(stderr, "Unable to set volume");
		}
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		_result = _selectedChannelGroup->getVolume(&volume);
		if (_result != FMOD_OK) {
			fprintf(stderr, "Unable to read volume");
		}
		//Clamp volume
		volume = (volume <= volumeMin) ? volumeMin : (volume - stepSize);
		_result = _selectedChannelGroup->setVolume(volume);
		if (_result != FMOD_OK) {
			fprintf(stderr, "Unable to set volume");
		}
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		_result = _selectedChannelGroup->getPitch(&pitch);
		if (_result != FMOD_OK) {
			fprintf(stderr, "Unable to get pitch");
		}
		//Clamp pitch
		pitch = (pitch >= pitchMax) ? pitchMax : (pitch + stepSize);
		//Set pitch
		_result = _selectedChannelGroup->setPitch(pitch);
		if (_result != FMOD_OK) {
			fprintf(stderr, "Unable to set pitch");
		}
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		_result = _selectedChannelGroup->getPitch(&pitch);
		if (_result != FMOD_OK) {
			fprintf(stderr, "Unable to get pitch");
		}
		//Clamp pitch
		pitch = (pitch <= pitchMin) ? pitchMin : (pitch - stepSize);
		//Set pitch
		_result = _selectedChannelGroup->setPitch(pitch);
		if (_result != FMOD_OK) {
			fprintf(stderr, "Unable to set pitch");
		}
	}
	else if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
		//you can do better
		if (zKey) {
			bool bypass = false;
			_result = _dspEcho->getBypass(&bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to get bypass!\n");
			}
			_result = _dspEcho->setBypass(!bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to set bypass!\n");
			}
		}
		else if (xKey) {
			bool bypass = false;
			_result = _dspDistortion->getBypass(&bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to get bypass!\n");
			}
			_result = _dspDistortion->setBypass(!bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to set bypass!\n");
			}

		}else if (cKey) {
			bool bypass = false;
			_result = _dspChorus->getBypass(&bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to get bypass!\n");
			}
			_result = _dspChorus->setBypass(!bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to set bypass!\n");
			}

		}else if (vKey) {
			bool bypass = false;
			_result = _dspDelay->getBypass(&bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to get bypass!\n");
			}
			_result = _dspDelay->setBypass(!bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to set bypass!\n");
			}
		}
		
	}
	else if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
		////you can do better
		if (zKey) {
			bool bypass = false;
			_result = _dspFlange->getBypass(&bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to get bypass!\n");
			}
			_result = _dspFlange->setBypass(!bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to set bypass!\n");
			}
		}
		else if (xKey) {
			bool bypass = false;
			_result = _dspTremolo->getBypass(&bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to get bypass!\n");
			}
			_result = _dspTremolo->setBypass(!bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to set bypass!\n");
			}

		}
		else if (cKey) {
			bool bypass = false;
			_result = _dspPan->getBypass(&bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to get bypass!\n");
			}
			_result = _dspPan->setBypass(!bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to set bypass!\n");
			}

		}
		else if (vKey) {
			bool bypass = false;
			_result = _dspConvolutionreverb->getBypass(&bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to get bypass!\n");
			}
			_result = _dspConvolutionreverb->setBypass(!bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to set bypass!\n");
			}
		}
	}
	else if (key == GLFW_KEY_8 && action == GLFW_PRESS) {
		if (zKey) {
			bool bypass = false;
			_result = _dspObjectpan->getBypass(&bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to get bypass!\n");
			}
			_result = _dspObjectpan->setBypass(!bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to set bypass!\n");
			}
		}
		else if (xKey) {
			bool bypass = false;
			_result = _dspOscillator->getBypass(&bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to get bypass!\n");
			}
			_result = _dspOscillator->setBypass(!bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to set bypass!\n");
			}

		}
		else if (cKey) {
			bool bypass = false;
			_result = _dspFader->getBypass(&bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to get bypass!\n");
			}
			_result = _dspFader->setBypass(!bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to set bypass!\n");
			}

		}
		else if (vKey) {
			bool bypass = false;
			_result = _dspLowpass->getBypass(&bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to get bypass!\n");
			}
			_result = _dspLowpass->setBypass(!bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to set bypass!\n");
			}
		}
	}
	else if (key == GLFW_KEY_7 && action == GLFW_PRESS) {
		if (zKey) {
			bool bypass = false;
			_result = _dspHighpass->getBypass(&bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to get bypass!\n");
			}
			_result = _dspHighpass->setBypass(!bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to set bypass!\n");
			}
		}
		else if (xKey) {
			bool bypass = false;
			_result = _dspNormalize->getBypass(&bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to get bypass!\n");
			}
			_result = _dspNormalize->setBypass(!bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to set bypass!\n");
			}

		}
		else if (cKey) {
			bool bypass = false;
			_result = _dspLimiter->getBypass(&bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to get bypass!\n");
			}
			_result = _dspLimiter->setBypass(!bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to set bypass!\n");
			}

		}
		else if (vKey) {
			bool bypass = false;
			_result = _dspCompressor->getBypass(&bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to get bypass!\n");
			}
			_result = _dspCompressor->setBypass(!bypass);
			if (FMOD_OK != _result) {
				fprintf(stderr, "Unable to set bypass!\n");
			}
		}
	}
}


void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "FMOD Error(%d): %s", error, description);
}

bool init();
bool initGLFW();
bool initGL();
bool initFMOD();
void shutdown();
void releaseAllSound();
void shutdownSound(FMOD::Sound* _soundToRelease);


//to get sound from file
std::vector<std::string> getSoundFromFile(std::string fileName);


//Main Function
int main(int args, char* argv) {
	//fprintf(stdout,"Hello world");
	if (!init()) {
		fprintf(stderr, "Unable to initialize app");
		exit(EXIT_FAILURE);
	}

	std::stringstream ss;
	float volume = 0.0f;
	float pitch = 0.0f;
	bool isPaused = false;

	GLuint lastKnownRowIndex = _textRowIndex;

	//Main loop
	while (!glfwWindowShouldClose(_window)) {
		//passing color values to change background
		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lastKnownRowIndex = _textRowIndex;

		//Initial starting game
		_text->addLine("Press \"Enter\" to start the game", lastKnownRowIndex++);


		_text->addLine("AUDIO CONTROL STATUS  ", lastKnownRowIndex + 5);
		_result = _selectedChannelGroup->getName(_buffer, BUFFER_SIZE);
		if (_result != FMOD_OK) 
			fprintf(stderr, "Unable to read selected channel group name");
		ss.str("");
		ss << "SELECTED GROUP NAME : " << _buffer;
		_text->addLine(ss.str(), lastKnownRowIndex+6);

		_result = _selectedChannelGroup->getPaused(&isPaused);
		if (_result != FMOD_OK) 
			fprintf(stderr, "Unable to read selected channel group pause");
		ss.str("");
		ss << "SELECTED GROUP PAUSE : " << ((isPaused) ? "Yes" : "No");
		_text->addLine(ss.str(), lastKnownRowIndex+7);

		_result = _selectedChannelGroup->getVolume(&volume);
		if (_result != FMOD_OK) 
			fprintf(stderr, "Unable to read selected channel group volume");
		ss.str("");
		ss.precision(2);
		ss << "SELECT GROUP VOLUME : " << volume;
		_text->addLine(ss.str(), lastKnownRowIndex+8);

		_result = _selectedChannelGroup->getPitch(&pitch);
		if (_result != FMOD_OK) 
			fprintf(stderr, "Unable to read selected channel group pitch");
		ss.str("");
		ss.precision(2);
		ss << "SELECTED GROUP PITCH" << pitch;
		_text->addLine(ss.str(), lastKnownRowIndex+9);

		bool bypass = false;

		if (zKey) {

			_text->addLine("DSP STATUS", lastKnownRowIndex + 10);
			_dspEcho->getBypass(&bypass);
			ss.str("");
			ss << "Echo Active " << ((!bypass) ? "Yes" : "No");
			_text->addLine(ss.str(), lastKnownRowIndex + 11);

			_dspFlange->getBypass(&bypass);
			ss.str("");
			ss << "Flange Active " << ((!bypass) ? "Yes" : "No");
			_text->addLine(ss.str(), lastKnownRowIndex + 12);

			_dspObjectpan->getBypass(&bypass);
			ss.str("");
			ss << "Object Pan Active " << ((!bypass) ? "Yes" : "No");
			_text->addLine(ss.str(), lastKnownRowIndex + 13);

			_dspHighpass->getBypass(&bypass);
			ss.str("");
			ss << "High Pass Active " << ((!bypass) ? "Yes" : "No");
			_text->addLine(ss.str(), lastKnownRowIndex + 14);

		}
		else if (xKey) {
			_text->addLine("DSP STATUS", lastKnownRowIndex + 10);


			_dspDistortion->getBypass(&bypass);
			ss.str("");
			ss << "Distortion Active " << ((!bypass) ? "Yes" : "No");
			_text->addLine(ss.str(), lastKnownRowIndex + 11);

			_dspTremolo->getBypass(&bypass);
			ss.str("");
			ss << "Tremolo Active " << ((!bypass) ? "Yes" : "No");
			_text->addLine(ss.str(), lastKnownRowIndex + 12);

			_dspOscillator->getBypass(&bypass);
			ss.str("");
			ss << "Oscillator Active " << ((!bypass) ? "Yes" : "No");
			_text->addLine(ss.str(), lastKnownRowIndex + 13);

			_dspNormalize->getBypass(&bypass);
			ss.str("");
			ss << "Normalize Active " << ((!bypass) ? "Yes" : "No");
			_text->addLine(ss.str(), lastKnownRowIndex + 14);
		}
		else if (cKey) {
			_text->addLine("DSP STATUS", lastKnownRowIndex + 10);

			_dspChorus->getBypass(&bypass);
			ss.str("");
			ss << "Chorus Active " << ((!bypass) ? "Yes" : "No");
			_text->addLine(ss.str(), lastKnownRowIndex + 11);

			_dspPan->getBypass(&bypass);
			ss.str("");
			ss << "Pan Active " << ((!bypass) ? "Yes" : "No");
			_text->addLine(ss.str(), lastKnownRowIndex + 12);

			_dspFader->getBypass(&bypass);
			ss.str("");
			ss << "Fader Active " << ((!bypass) ? "Yes" : "No");
			_text->addLine(ss.str(), lastKnownRowIndex + 13);

			_dspLimiter->getBypass(&bypass);
			ss.str("");
			ss << "Limiter Active " << ((!bypass) ? "Yes" : "No");
			_text->addLine(ss.str(), lastKnownRowIndex + 14);

		}
		else if (vKey) {
			_text->addLine("DSP STATUS", lastKnownRowIndex + 10);

			_dspDelay->getBypass(&bypass);
			ss.str("");
			ss << "Delay Active " << ((!bypass) ? "Yes" : "No");
			_text->addLine(ss.str(), lastKnownRowIndex + 11);

			_dspConvolutionreverb->getBypass(&bypass);
			ss.str("");
			ss << "Convolution Reverb Active " << ((!bypass) ? "Yes" : "No");
			_text->addLine(ss.str(), lastKnownRowIndex + 12);

			_dspLowpass->getBypass(&bypass);
			ss.str("");
			ss << "Low Pass Active " << ((!bypass) ? "Yes" : "No");
			_text->addLine(ss.str(), lastKnownRowIndex + 13);

			_dspCompressor->getBypass(&bypass);
			ss.str("");
			ss << "Compressor Active " << ((!bypass) ? "Yes" : "No");
			_text->addLine(ss.str(), lastKnownRowIndex + 14);

		}


		_text->addLine("AUDIO GROUP CONTROLS", lastKnownRowIndex + 16);
		_text->addLine("P - to pause a Selected Channel, Up/Down Increase and decrease Volume,Left/Right Increase and decrease Pitch", lastKnownRowIndex + 17);
		_text->addLine("Z to select BGM Channel,  X to select Question Voice Channel, C to select Options for Questions Channel", lastKnownRowIndex + 18);

		_text->addLine("CONTROLS", lastKnownRowIndex + 19);
		_text->addLine("ENTER - to start the game, ESC to quit game", lastKnownRowIndex + 20);
		_text->addLine("Q - Replay the given beat, R - Replay the given beat options AND 1, 2, 3, 4 - Options to guess the beat", lastKnownRowIndex + 21);

		//LEVEL 1
		if (isStartgame) {
			_text->addLine("LEVEL 1", lastKnownRowIndex++);
			_text->addLine("Identify the sound played. You have 4 option to answer. Press Q to hear the given beat. Press R to hear 4 options of sound. Click 1,2,3 or 4 to select the options. You have 3 attempts to answer", lastKnownRowIndex++);
		}
		//LEVEL 2
		else if (isLevel1) {
			_text->addLine("LEVEL 2", lastKnownRowIndex);
			_text->addLine("Identify which side the sound played. You have 3 option to answer. Press Q to hear the given beat. Press R to hear 3 options of sound. Click 1,2 or 3 to select the options. You have 3 attempts to answer", ++lastKnownRowIndex);

		}
		//LEVEL 3
		else if (isLevel2) {
			_text->addLine("LEVEL 3", lastKnownRowIndex);
			_text->addLine("Identify the matching sound. You have 3 option to answer. Press Q to play the sound. Press R to hear 3 variations of the sound. Which did it match? Click 1,2 or 3. You have 3 attempts to answer ", ++lastKnownRowIndex);

		}
		//Game won
		else if (isLevel3) {
			_text->addLine("CONGRATULATIONS!", lastKnownRowIndex);
			_text->addLine("Looks like we have a winner with a keen ear! Press Esc to Exit. ", ++lastKnownRowIndex);
			_text->addLine("END CREDITS", lastKnownRowIndex + 5);
			_text->addLine("Pradeep & Dennison", lastKnownRowIndex + 6);
			for (int i = 7; i < 22;i++) {
				_text->addLine(" ", lastKnownRowIndex + i);
			}


		}
		if (isEndgame) {
			_text->addLine("Game Over", --lastKnownRowIndex);
			_text->addLine("You Lose", lastKnownRowIndex);
			_text->addLine("Press Esc to quit. Relaunch to try again.", ++lastKnownRowIndex);
			_text->addLine(" ", ++lastKnownRowIndex);
			_text->addLine("END CREDITS", lastKnownRowIndex + 5);
			_text->addLine("Pradeep & Dennison", lastKnownRowIndex + 6);
			for (int i = 7; i < 22; i++) {
				_text->addLine(" ", lastKnownRowIndex + i);
			}
		}








		
		







		_text->render();
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
	shutdown();

	return 0;
}

bool init() {

	if (!initGLFW()) {
		return false;
	}
	if (!initGL()) {
		return false;
	}
	//dependency GLtext and Opengl
	_text = new RSS::GLText(__applicationName, _width, _height);

	if (!initFMOD()) {
		return false;
	}
	return true;
}

bool initGLFW() {
	if (!glfwInit()) {
		fprintf(stderr, "Unable to initalize GLFW");
		return false;
	}

	//error callback
	glfwSetErrorCallback(glfw_error_callback);

	_window = glfwCreateWindow(_width, _height, __applicationName.c_str(), NULL, NULL);
	if (!_window) {
		fprintf(stderr, "Unable to create GLFW window");
		return false;
	}

	//set keyboard callback
	glfwSetKeyCallback(_window, glfw_key_callback);
	glfwMakeContextCurrent(_window);
	glfwSwapInterval(1);

	return true;
}

bool initGL() {
	if (!gladLoadGL(glfwGetProcAddress)) {
		fprintf(stderr, "unable to initialize glad");
		return false;
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

bool initFMOD() {
	_result = FMOD::System_Create(&_system);
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to create FMOD system");
		return false;
	}
	_result = _system->init(32, FMOD_INIT_NORMAL, NULL);
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to initialize FMOD system");
		return false;
	}



	//Create groups
	_result = _system->createChannelGroup("Group BGM", &_channelGroupBGM);
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to create channel group");
		return false;
	}

	_result = _system->createChannelGroup("Group Voice", &_channelGroupVoice);
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to create channel group");
		return false;
	}

	_result = _system->createChannelGroup("Group Option", &_channelGroupOption);
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to create channel group");
		return false;
	}

	_result = _system->createChannelGroup("Group Song", &_channelGroupSong);
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to create channel group");
		return false;
	}

	//Get master channel
	_result = _system->getMasterChannelGroup(&_channelGroupMaster);
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to get master channel group");
		return false;
	}


	//AudioBGM text file
	_audioBGM = getSoundFromFile("audioBGM.txt");
	std::stringstream bgm[4];
	for (int i = 0; i < _audioBGM.size(); i++) {
		//_file using external
		bgm[i] << SOLUTION_DIR << "common\\assets\\audio\\" << _audioBGM.at(i);
	}
	_result = _system->createSound(bgm[0].str().c_str(), FMOD_LOOP_NORMAL, 0, &_soundStartBgm);
	_result = _system->createSound(bgm[1].str().c_str(), FMOD_LOOP_NORMAL, 0, &_soundMidBgm);
	_result = _system->createSound(bgm[2].str().c_str(), FMOD_LOOP_NORMAL, 0, &_soundEndBgm);


	/*LEVEL 1 starts*/
	//AudioQuestion LEVEL 1 text file
	_audioQuestionLevel1 = getSoundFromFile("audioQuestionLevel1.txt");
	std::stringstream questionsLevel1[4];
	for (int i = 0; i < _audioQuestionLevel1.size(); i++) {
		//_file using external
		questionsLevel1[i] << SOLUTION_DIR << "common\\assets\\audio\\" << _audioQuestionLevel1.at(i);
	}
	_result = _system->createSound(questionsLevel1[0].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestionSound);
	_result = _system->createSound(questionsLevel1[1].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestion1Sound);
	_result = _system->createSound(questionsLevel1[2].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestion2Sound);
	_result = _system->createSound(questionsLevel1[3].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestion3Sound);
	/*LEVEL 1 ends*/


	/*LEVEL 2 and 3 starts*/
	//AudioQuestion LEVEL 2 and LEVEL 3 text file
	_audioQuestionLevel2and3 = getSoundFromFile("audioQuestionLevel2and3.txt");
	std::stringstream questionLevel2and3[2];
	for (int i = 0; i < _audioQuestionLevel2and3.size(); i++) {
		//_file using external
		questionLevel2and3[i] << SOLUTION_DIR << "common\\assets\\audio\\" << _audioQuestionLevel2and3.at(i);
	}
	_result = _system->createSound(questionLevel2and3[0].str().c_str(), FMOD_LOOP_OFF, 0, &_soundLevel2Sound);
	_result = _system->createSound(questionLevel2and3[1].str().c_str(), FMOD_LOOP_OFF, 0, &_soundLevel3Sound);
	/*LEVEL 2 and 3 ends*/


	/*Automated sound starts*/
	//Audio for automated sound
	_audioAutomatedSound = getSoundFromFile("audioAutomatedSound.txt");
	std::stringstream audioAutomatedSound[7];
	for (int i = 0; i < _audioAutomatedSound.size(); i++) {
		//_file using external
		audioAutomatedSound[i] << SOLUTION_DIR << "common\\assets\\audio\\" << _audioAutomatedSound.at(i);
	}
	_result = _system->createSound(audioAutomatedSound[0].str().c_str(), FMOD_LOOP_OFF, 0, &_soundLevel2Left);
	_result = _system->createSound(audioAutomatedSound[1].str().c_str(), FMOD_LOOP_OFF, 0, &_soundLevel2Center);
	_result = _system->createSound(audioAutomatedSound[2].str().c_str(), FMOD_LOOP_OFF, 0, &_soundLevel2Right);
	_result = _system->createSound(audioAutomatedSound[3].str().c_str(), FMOD_LOOP_OFF, 0, &_soundClick);
	_result = _system->createSound(audioAutomatedSound[4].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestion1);
	_result = _system->createSound(audioAutomatedSound[5].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestion2);
	_result = _system->createSound(audioAutomatedSound[6].str().c_str(), FMOD_LOOP_OFF, 0, &_soundQuestion3);
	/*Automated sound ends*/



	/*Game win or lose starts*/
	//Audio for game won/lose
	_audioGameWonOrLose = getSoundFromFile("audioGameWonOrLose.txt");
	std::stringstream audioGameWonOrLose[2];
	for (int i = 0; i < _audioGameWonOrLose.size(); i++) {
		//_file using external
		audioGameWonOrLose[i] << SOLUTION_DIR << "common\\assets\\audio\\" << _audioGameWonOrLose.at(i);
	}
	_result = _system->createSound(audioGameWonOrLose[1].str().c_str(), FMOD_LOOP_OFF, 0, &_soundGameLost);
	_result = _system->createSound(audioGameWonOrLose[0].str().c_str(), FMOD_LOOP_OFF, 0, &_soundGameWon);
	/*Game win or lose ends*/




	//_bgm startbgm
	//Initial Play start BGm
	_result = _system->playSound(_soundStartBgm, _channelGroupBGM, false, &_channelStartBgm);
	_channelStartBgm->setVolume(0.09);
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to play sound");
		return false;
	}



	_selectedChannelGroup = _channelGroupMaster;


	return true;
}


void shutdown() {
	glfwTerminate();

	//single function to release all sound
	releaseAllSound();

	_result = _system->close();
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to close system");
	}

	_result = _system->release();
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to release system");
	}

	if (_text) {
		delete _text;
		_text = nullptr;
	}

	exit(EXIT_SUCCESS);
}

void releaseAllSound() {
	shutdownSound(_soundClick);
	shutdownSound(_soundEndBgm);
	shutdownSound(_soundGameLost);
	shutdownSound(_soundGameWon);
	shutdownSound(_soundLevel2Center);
	shutdownSound(_soundLevel2Left);
	shutdownSound(_soundLevel2Right);
	shutdownSound(_soundLevel2Sound);
	shutdownSound(_soundLevel3Sound);
	shutdownSound(_soundMidBgm);
	shutdownSound(_soundQuestion1);
	shutdownSound(_soundQuestion1Sound);
	shutdownSound(_soundQuestion2);
	shutdownSound(_soundQuestion2Sound);
	shutdownSound(_soundQuestion3);
	shutdownSound(_soundQuestion3Sound);
	shutdownSound(_soundQuestionSound);
	shutdownSound(_soundStartBgm);
}

void shutdownSound(FMOD::Sound* _soundToRelease) {
	_result = _soundToRelease->release();
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to release sound");
	}
}

//Readfile and get the path
std::vector<std::string> getSoundFromFile(std::string fileName) {

	std::vector<std::string> result;
	std::stringstream ss;
	ss << SOLUTION_DIR << "common\\assets\\audio\\" << fileName;

	//read file 
	std::ifstream inputFile(ss.str());
	std::string line;
	while (std::getline(inputFile, line)) {
		result.push_back(line);
	}

	return result;
}
