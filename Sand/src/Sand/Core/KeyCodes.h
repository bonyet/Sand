#pragma once

namespace Sand 
{
	enum class Keycode : uint32_t
	{
		Space      = 32u,
		Apostrophe = 39u,
		Comma      = 44u,
		Minus      = 45u,
		Period     = 46u,
		Slash      = 47u,

		A_0 = 48u,
		A_1 = 49u,
		A_2 = 50u,
		A_3 = 51u,
		A_4 = 52u,
		A_5 = 53u,
		A_6 = 54u,
		A_7 = 55u,
		A_8 = 56u,
		A_9 = 57u,

		Semicolon = 59u,
		Equal     = 61u,

		A = 65u,
		B = 66u,
		C = 67u,
		D = 68u,
		E = 69u,
		F = 70u,
		G = 71u,
		H = 72u,
		I = 73u,
		J = 74u,
		K = 75u,
		L = 76u,
		M = 77u,
		N = 78u,
		O = 79u,
		P = 80u,
		Q = 81u,
		R = 82u,
		S = 83u,
		T = 84u,
		U = 85u,
		V = 86u,
		W = 87u,
		X = 88u,
		Y = 89u,
		Z = 90u,

		LeftBracket  = 91u,
		Backslash     = 92u,
		RightBracket = 93u,
		Grave  = 96u,
		World1       = 161u,
		World2       = 162u,

		Escape       = 256u,
		Enter        = 257u,
		Tab          = 258u,
		Backspace    = 259u,
		Insert       = 260u,
		Delete       = 261u,
		RightArrow   = 262u,
		LeftArrow    = 263u,
		DownArrow    = 264u,
		UpArrow      = 265u,
		PageUp       = 266u,
		PageDown     = 267u,
		Home         = 268u,
		End          = 269u,
		CapsLock     = 280u,
		ScrollLock   = 281u,
		NumLock      = 282u,
		PrintScreen  = 283u,
		Pause        = 284u,

		F1  = 290u,
		F2  = 291u,
		F3  = 292u,
		F4  = 293u,
		F5  = 294u,
		F6  = 295u,
		F7  = 296u,
		F8  = 297u,
		F9  = 298u,
		F10 = 299u,
		F11 = 300u,
		F12 = 301u,
		F13 = 302u,
		F14 = 303u,
		F15 = 304u,
		F16 = 305u,
		F17 = 306u,
		F18 = 307u,
		F19 = 308u,
		F20 = 309u,
		F21 = 310u,
		F22 = 311u,
		F23 = 312u,
		F24 = 313u,
		F25 = 314u,

		KP_0 = 320u,
		KP_1 = 321u,
		KP_2 = 322u,
		KP_3 = 323u,
		KP_4 = 324u,
		KP_5 = 325u,
		KP_6 = 326u,
		KP_7 = 327u,
		KP_8 = 328u,
		KP_9 = 329u,
		KP_Decimal  = 330u,
		KP_Divide   = 331u,
		KP_Multiply = 332u,
		KP_Subtract = 333u,
		KP_Add      = 334u,
		KP_Enter    = 335u,
		KP_Equal    = 336u,

		LeftShift    = 340u,
		LeftControl  = 341u,
		LeftAlt      = 342u,
		LeftSuper    = 343u,
		RightShift   = 344u,
		RightControl = 345u,
		RightAlt     = 346u,
		RightSuper   = 347u,
		Menu         = 348u
	};

	inline std::ostream& operator<<(std::ostream& os, Keycode Keycode)
	{
		os << static_cast<uint32_t>(Keycode);
		return os;
	}
}

#define SAND_KEY_SPACE           Sand::Keycode::SPACE        
#define SAND_KEY_APOSTROPHE      Sand::Keycode::APOSTROPHE   
#define SAND_KEY_COMMA           Sand::Keycode::COMMA        
#define SAND_KEY_MINUS           Sand::Keycode::MINUS        
#define SAND_KEY_PERIOD          Sand::Keycode::PERIOD       
#define SAND_KEY_SLASH           Sand::Keycode::SLASH        

#define SAND_KEY_0               Sand::Keycode::DP_0            
#define SAND_KEY_1               Sand::Keycode::DP_1            
#define SAND_KEY_2               Sand::Keycode::DP_2            
#define SAND_KEY_3               Sand::Keycode::DP_3            
#define SAND_KEY_4               Sand::Keycode::DP_4            
#define SAND_KEY_5               Sand::Keycode::DP_5            
#define SAND_KEY_6               Sand::Keycode::DP_6            
#define SAND_KEY_7               Sand::Keycode::DP_7            
#define SAND_KEY_8               Sand::Keycode::DP_8            
#define SAND_KEY_9               Sand::Keycode::DP_9            

#define SAND_KEY_SEMICOLON       Sand::Keycode::SEMICOLON    
#define SAND_KEY_EQUAL           Sand::Keycode::EQUAL        

#define SAND_KEY_A               Sand::Keycode::A            
#define SAND_KEY_B               Sand::Keycode::B            
#define SAND_KEY_C               Sand::Keycode::C            
#define SAND_KEY_D               Sand::Keycode::D            
#define SAND_KEY_E               Sand::Keycode::E            
#define SAND_KEY_F               Sand::Keycode::F            
#define SAND_KEY_G               Sand::Keycode::G            
#define SAND_KEY_H               Sand::Keycode::H            
#define SAND_KEY_I               Sand::Keycode::I            
#define SAND_KEY_J               Sand::Keycode::J            
#define SAND_KEY_K               Sand::Keycode::K            
#define SAND_KEY_L               Sand::Keycode::L            
#define SAND_KEY_M               Sand::Keycode::M            
#define SAND_KEY_N               Sand::Keycode::N            
#define SAND_KEY_O               Sand::Keycode::O            
#define SAND_KEY_P               Sand::Keycode::P            
#define SAND_KEY_Q               Sand::Keycode::Q            
#define SAND_KEY_R               Sand::Keycode::R            
#define SAND_KEY_S               Sand::Keycode::S            
#define SAND_KEY_T               Sand::Keycode::T            
#define SAND_KEY_U               Sand::Keycode::U            
#define SAND_KEY_V               Sand::Keycode::V            
#define SAND_KEY_W               Sand::Keycode::W            
#define SAND_KEY_X               Sand::Keycode::X            
#define SAND_KEY_Y               Sand::Keycode::Y            
#define SAND_KEY_Z               Sand::Keycode::Z            

#define SAND_KEY_LEFT_BRACKET    Sand::Keycode::LEFT_BRACKET 
#define SAND_KEY_BACKSLASH       Sand::Keycode::BACKSLASH    
#define SAND_KEY_RIGHT_BRACKET   Sand::Keycode::RIGHT_BRACKET
#define SAND_KEY_GRAVE_ACCENT    Sand::Keycode::GRAVE_ACCENT 
#define SAND_KEY_WORLD_1         Sand::Keycode::WORLD_1      
#define SAND_KEY_WORLD_2         Sand::Keycode::WORLD_2      

#define SAND_KEY_ESCAPE          Sand::Keycode::ESCAPE       
#define SAND_KEY_ENTER           Sand::Keycode::ENTER        
#define SAND_KEY_TAB             Sand::Keycode::TAB          
#define SAND_KEY_BACKSPACE       Sand::Keycode::BACKSPACE    
#define SAND_KEY_INSERT          Sand::Keycode::INSERT       
#define SAND_KEY_DELETE          Sand::Keycode::DEL       
#define SAND_KEY_RIGHT           Sand::Keycode::RIGHT        
#define SAND_KEY_LEFT            Sand::Keycode::LEFT         
#define SAND_KEY_DOWN            Sand::Keycode::DOWN         
#define SAND_KEY_UP              Sand::Keycode::UP           
#define SAND_KEY_PAGE_UP         Sand::Keycode::PAGE_UP      
#define SAND_KEY_PAGE_DOWN       Sand::Keycode::PAGE_DOWN    
#define SAND_KEY_HOME            Sand::Keycode::HOME         
#define SAND_KEY_END             Sand::Keycode::END          
#define SAND_KEY_CAPS_LOCK       Sand::Keycode::CAPS_LOCK    
#define SAND_KEY_SCROLL_LOCK     Sand::Keycode::SCROLL_LOCK  
#define SAND_KEY_NUM_LOCK        Sand::Keycode::NUM_LOCK     
#define SAND_KEY_PRINT_SCREEN    Sand::Keycode::PRINT_SCREEN 
#define SAND_KEY_PAUSE           Sand::Keycode::PAUSE        

#define SAND_KEY_F1              Sand::Keycode::F1           
#define SAND_KEY_F2              Sand::Keycode::F2           
#define SAND_KEY_F3              Sand::Keycode::F3           
#define SAND_KEY_F4              Sand::Keycode::F4           
#define SAND_KEY_F5              Sand::Keycode::F5           
#define SAND_KEY_F6              Sand::Keycode::F6           
#define SAND_KEY_F7              Sand::Keycode::F7           
#define SAND_KEY_F8              Sand::Keycode::F8           
#define SAND_KEY_F9              Sand::Keycode::F9           
#define SAND_KEY_F10             Sand::Keycode::F10          
#define SAND_KEY_F11             Sand::Keycode::F11          
#define SAND_KEY_F12             Sand::Keycode::F12          
#define SAND_KEY_F13             Sand::Keycode::F13          
#define SAND_KEY_F14             Sand::Keycode::F14          
#define SAND_KEY_F15             Sand::Keycode::F15          
#define SAND_KEY_F16             Sand::Keycode::F16          
#define SAND_KEY_F17             Sand::Keycode::F17          
#define SAND_KEY_F18             Sand::Keycode::F18          
#define SAND_KEY_F19             Sand::Keycode::F19          
#define SAND_KEY_F20             Sand::Keycode::F20          
#define SAND_KEY_F21             Sand::Keycode::F21          
#define SAND_KEY_F22             Sand::Keycode::F22          
#define SAND_KEY_F23             Sand::Keycode::F23          
#define SAND_KEY_F24             Sand::Keycode::F24          
#define SAND_KEY_F25             Sand::Keycode::F25          

#define SAND_KEY_KP_0            Sand::Keycode::KP_0         
#define SAND_KEY_KP_1            Sand::Keycode::KP_1         
#define SAND_KEY_KP_2            Sand::Keycode::KP_2         
#define SAND_KEY_KP_3            Sand::Keycode::KP_3         
#define SAND_KEY_KP_4            Sand::Keycode::KP_4         
#define SAND_KEY_KP_5            Sand::Keycode::KP_5         
#define SAND_KEY_KP_6            Sand::Keycode::KP_6         
#define SAND_KEY_KP_7            Sand::Keycode::KP_7         
#define SAND_KEY_KP_8            Sand::Keycode::KP_8         
#define SAND_KEY_KP_9            Sand::Keycode::KP_9         
#define SAND_KEY_KP_DECIMAL      Sand::Keycode::KP_DECIMAL   
#define SAND_KEY_KP_DIVIDE       Sand::Keycode::KP_DIVIDE    
#define SAND_KEY_KP_MULTIPLY     Sand::Keycode::KP_MULTIPLY  
#define SAND_KEY_KP_SUBTRACT     Sand::Keycode::KP_SUBTRACT  
#define SAND_KEY_KP_ADD          Sand::Keycode::KP_ADD       
#define SAND_KEY_KP_ENTER        Sand::Keycode::KP_ENTER     
#define SAND_KEY_KP_EQUAL        Sand::Keycode::KP_EQUAL     

#define SAND_KEY_LEFT_SHIFT      Sand::Keycode::LEFT_SHIFT   
#define SAND_KEY_LEFT_CONTROL    Sand::Keycode::LEFT_CONTROL 
#define SAND_KEY_LEFT_ALT        Sand::Keycode::LEFT_ALT     
#define SAND_KEY_LEFT_SUPER      Sand::Keycode::LEFT_SUPER   
#define SAND_KEY_RIGHT_SHIFT     Sand::Keycode::RIGHT_SHIFT  
#define SAND_KEY_RIGHT_CONTROL   Sand::Keycode::RIGHT_CONTROL
#define SAND_KEY_RIGHT_ALT       Sand::Keycode::RIGHT_ALT    
#define SAND_KEY_RIGHT_SUPER     Sand::Keycode::RIGHT_SUPER  
#define SAND_KEY_MENU            Sand::Keycode::MENU         
