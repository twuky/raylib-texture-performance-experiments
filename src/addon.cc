#include <napi.h>
#include <raylib.h>

using namespace Napi;

// type conversions
Color ColorFromNAPIObject(Napi::Object obj) {
  Color c = Color();
  c.r = obj.Get("r").As<Napi::Number>().Uint32Value();
  c.g = obj.Get("g").As<Napi::Number>().Uint32Value();
  c.b = obj.Get("b").As<Napi::Number>().Uint32Value();
  c.a = obj.Get("a").As<Napi::Number>().Uint32Value();
  return c;
}

Texture TextureFromNAPIObject(Napi::Object obj) {
	Texture out = Texture();
	out.id = obj.Get("id").As<Napi::Number>();
	out.width = obj.Get("width").As<Napi::Number>();
	out.height = obj.Get("height").As<Napi::Number>();
	out.mipmaps = obj.Get("mipmaps").As<Napi::Number>();
	out.format = obj.Get("format").As<Napi::Number>();
	return out;
}

// function to create a "color". JS users can simply define an object with rgba properties
Napi::Object BindColor(const Napi::CallbackInfo& info) {
  Color c = Color();
  c.r = info[0].As<Napi::Number>().Uint32Value();
  c.g = info[1].As<Napi::Number>().Uint32Value();
  c.b = info[2].As<Napi::Number>().Uint32Value();
  c.a = info[3].As<Napi::Number>().Uint32Value();
  Napi::Object out = Napi::Object::New(info.Env());
  out.Set("r", c.r);
  out.Set("g", c.g);
  out.Set("b", c.b);
  out.Set("a", c.a);
  return out;
}

Napi::Object TextureToNAPIObject(napi_env env, Texture object) {
	Napi::Object out = Napi::Object::New(env);
	out.Set("id", object.id);
	out.Set("width", object.width);
	out.Set("height", object.height);
	out.Set("mipmaps", object.mipmaps);
	out.Set("format", object.format);
	return out;
}

// function binding wrappers
void BindInitWindow(const Napi::CallbackInfo& info) {
  return InitWindow(
    info[0].As<Napi::Number>(),
    info[1].As<Napi::Number>(),
    info[2].As<Napi::String>().Utf8Value().c_str()
  );
}

void BindSetTargetFPS(const Napi::CallbackInfo& info) {
  return SetTargetFPS(
    info[0].As<Napi::Number>()
  );
}

Napi::Boolean BindWindowShouldClose(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), WindowShouldClose());
}

void BindBeginDrawing(const Napi::CallbackInfo& info) {
  return BeginDrawing();
}

void BindEndDrawing(const Napi::CallbackInfo& info) {
  return EndDrawing();
}

Napi::Object BindLoadTexture(const Napi::CallbackInfo& info) {
  return TextureToNAPIObject(info.Env(), LoadTexture(info[0].As<Napi::String>().Utf8Value().c_str()));
}

void BindClearBackground(const Napi::CallbackInfo& info) {
  ClearBackground(ColorFromNAPIObject(info[0].As<Napi::Object>()));
}

void BindDrawText(const Napi::CallbackInfo& info) {
  DrawText(
    info[0].As<Napi::String>().Utf8Value().c_str(),
    info[1].As<Napi::Number>(),
    info[2].As<Napi::Number>(),
    info[3].As<Napi::Number>(),
    ColorFromNAPIObject(info[4].As<Napi::Object>())
  );
}

void BindDrawFPS(const Napi::CallbackInfo& info) {
  DrawFPS(
    info[0].As<Napi::Number>(),
    info[1].As<Napi::Number>()
  );
}

Napi::Number BindGetFPS(const Napi::CallbackInfo& info) {
  return Napi::Number::New(info.Env(), GetFPS());
}

void BindDrawRectangle(const Napi::CallbackInfo& info) {
  DrawRectangle(
    info[0].As<Napi::Number>(),
    info[1].As<Napi::Number>(),
    info[2].As<Napi::Number>(),
    info[3].As<Napi::Number>(),
    ColorFromNAPIObject(info[4].As<Napi::Object>())
  );
}

void BindDrawTexture(const Napi::CallbackInfo& info) {
	DrawTexture(
		TextureFromNAPIObject(info[0].As<Napi::Object>()),
		info[1].As<Napi::Number>(),
		info[2].As<Napi::Number>(),
		ColorFromNAPIObject(info[3].As<Napi::Object>())
	);
}

void BindDrawTextures(const Napi::CallbackInfo& info) {
	Napi::Array array = info[0].As<Napi::Array>();
	for (uint32_t i = 0; i < array.Length(); i++) {
		Napi::Value val = array[i];
		Napi::Object obj = val.As<Napi::Object>();
		DrawTexture(
			TextureFromNAPIObject(obj.Get("texture").As<Napi::Object>()),
			obj.Get("x").As<Napi::Number>(),
			obj.Get("y").As<Napi::Number>(),
			ColorFromNAPIObject(obj.Get("color").As<Napi::Object>())
		);
	}
}

Napi::Object bunnies[20000];
int idx = 0;

void BindAddArray(const Napi::CallbackInfo& info) {
	bunnies[idx] = info[0].As<Napi::Object>();
	idx++;
}

void BindDrawBunnies(const Napi::CallbackInfo& info) {
	for (int i = 0; i < idx - 1; i++) {
		DrawTexture(
			TextureFromNAPIObject(bunnies[i].Get("texture").As<Napi::Object>()),
			bunnies[i].Get("x").As<Napi::Number>(),
			bunnies[i].Get("y").As<Napi::Number>(),
			ColorFromNAPIObject(bunnies[i].Get("color").As<Napi::Object>())
		);
	}
}

bool loaded = false;
Texture2D bunny_texture;

void BindDrawTextureShortWhite(const Napi::CallbackInfo& info) {
  DrawTexture(
		(Texture2D){
      info[0].As<Napi::Number>(),
		  info[1].As<Napi::Number>(),
      info[2].As<Napi::Number>(),
      },
		info[3].As<Napi::Number>(),
		info[4].As<Napi::Number>(),
		RAYWHITE
	);
}

void BindDrawTextureShort(const Napi::CallbackInfo& info) {
  DrawTexture(
		(Texture2D){
      info[0].As<Napi::Number>(),
		  info[1].As<Napi::Number>(),
      info[2].As<Napi::Number>(),
      },
		info[3].As<Napi::Number>(),
		info[4].As<Napi::Number>(),
		(Color){
      (int)info[5].As<Napi::Number>(),
      (int)info[6].As<Napi::Number>(),
      (int)info[7].As<Napi::Number>(),
      (int)info[8].As<Napi::Number>()
    }
	);
}

void BindDrawTextureInt(const Napi::CallbackInfo& info) {
  DrawTexture(
		(Texture2D){
      info[0].As<Napi::Number>(),
		  info[1].As<Napi::Number>(),
      info[2].As<Napi::Number>(),
      1,
      7
    },
		info[3].As<Napi::Number>(),
		info[4].As<Napi::Number>(),
    GetColor(info[5].As<Napi::Number>())
	);
}

void BindDrawTextureShortBuffer(const Napi::CallbackInfo& info) {
  Napi::Buffer<int8_t> buffer = info[5].As<Napi::Buffer<int8_t>>();
  DrawTexture(
		(Texture2D){
      info[0].As<Napi::Number>(),
		  info[1].As<Napi::Number>(),
      info[2].As<Napi::Number>(),
      },
		info[3].As<Napi::Number>(),
		info[4].As<Napi::Number>(),
		(Color){
      buffer[0],
      buffer[1],
      buffer[2],
      buffer[3]
    }
	);
}


void BindDrawBunnyWhite(const Napi::CallbackInfo& info) {
	DrawTexture(
		(Texture2D){3, 64, 64},
		info[0].As<Napi::Number>(),
		info[1].As<Napi::Number>(),
		RAYWHITE
	);
}

void BindDrawBunny(const Napi::CallbackInfo& info) {
	DrawTexture(
		(Texture2D){3, 64, 64},
		info[0].As<Napi::Number>(),
		info[1].As<Napi::Number>(),
		(Color){
      (int)info[2].As<Napi::Number>(),
      (int)info[3].As<Napi::Number>(),
      (int)info[4].As<Napi::Number>(),
      (int)info[5].As<Napi::Number>()
    }
	);
}

void BindDrawTextureFlat(const Napi::CallbackInfo& info) {

	DrawTexture(
		(Texture2D) {
      info[0].As<Napi::Number>(),
		  info[1].As<Napi::Number>(),
      info[2].As<Napi::Number>(),
		  info[3].As<Napi::Number>(),
      info[4].As<Napi::Number>(),
    },
		info[5].As<Napi::Number>(),
		info[6].As<Napi::Number>(),
		(Color){
      (int)info[7].As<Napi::Number>(),
      (int)info[8].As<Napi::Number>(),
      (int)info[9].As<Napi::Number>(),
      (int)info[10].As<Napi::Number>()
    }
	);
}

void BindDrawTextureFlatWhite(const Napi::CallbackInfo& info) {

	DrawTexture(
		(Texture2D) {
      info[0].As<Napi::Number>(),
		  info[1].As<Napi::Number>(),
      info[2].As<Napi::Number>(),
		  info[3].As<Napi::Number>(),
      info[4].As<Napi::Number>(),
    },
		info[5].As<Napi::Number>(),
		info[6].As<Napi::Number>(),
		RAYWHITE
	);
}

void BindDrawBunnyBuffer(const Napi::CallbackInfo& info) {  
  Napi::Buffer<float> buffer = info[0].As<Napi::Buffer<float>>();

	DrawTexture(
	  (Texture2D){3, 64, 64},
		buffer[0],
		buffer[1],
		(Color){
      (int)buffer[2],
      (int)buffer[3],
      (int)buffer[4],
      (int)buffer[5]
    }
	);
}

Napi::Object BindGetColor(const Napi::CallbackInfo& info) {
	Color obj = GetColor(
		info[0].As<Napi::Number>()
	);
	Napi::Object out = Napi::Object::New(info.Env());
	out.Set("r", obj.r);
	out.Set("g", obj.g);
	out.Set("b", obj.b);
	out.Set("a", obj.a);
	return out;
}

Napi::Number BindGetTexturePointer(const Napi::CallbackInfo& info) {
  Texture2D tex = TextureFromNAPIObject(info[0].As<Napi::Object>());
  void* ptr = MemAlloc(sizeof(Texture2D));
  *(Texture2D*)ptr = tex;
  return Napi::Number::New(info.Env(), (int64_t) ptr);
}

void BindDrawTexturePointer(const Napi::CallbackInfo& info) {
  DrawTexture(
	  *(Texture2D*)info[0].As<Napi::Number>().Int64Value(),
		info[1].As<Napi::Number>(),
		info[2].As<Napi::Number>(),
    GetColor(info[3].As<Napi::Number>())
	);
}

void BindDrawTexturePointerWhite(const Napi::CallbackInfo& info) {
  DrawTexture(
	  *(Texture2D*)info[0].As<Napi::Number>().Int64Value(),
		info[1].As<Napi::Number>(),
		info[2].As<Napi::Number>(),
    WHITE
	);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {

  exports.Set(
    Napi::String::New(env, "InitWindow"),
    Napi::Function::New(env, BindInitWindow)
  );

  exports.Set(
    Napi::String::New(env, "SetTargetFPS"),
    Napi::Function::New(env, BindSetTargetFPS)
  );

  exports.Set(
    Napi::String::New(env, "WindowShouldClose"),
    Napi::Function::New(env, BindWindowShouldClose)
  );

  exports.Set(
    Napi::String::New(env, "BeginDrawing"),
    Napi::Function::New(env, BindBeginDrawing)
  );

  exports.Set(
    Napi::String::New(env, "EndDrawing"),
    Napi::Function::New(env, BindEndDrawing)
  );

  exports.Set(
    Napi::String::New(env, "Color"),
    Napi::Function::New(env, BindColor)
  );

  exports.Set(
    Napi::String::New(env, "ClearBackground"),
    Napi::Function::New(env, BindClearBackground)
  );

  exports.Set(
    Napi::String::New(env, "DrawText"),
    Napi::Function::New(env, BindDrawText)
  );

  exports.Set(
    Napi::String::New(env, "DrawFPS"),
    Napi::Function::New(env, BindDrawFPS)
  );

  exports.Set(
    Napi::String::New(env, "LoadTexture"),
    Napi::Function::New(env, BindLoadTexture)
  );

  exports.Set(
    Napi::String::New(env, "GetFPS"),
    Napi::Function::New(env, BindGetFPS)
  );

  exports.Set(
    Napi::String::New(env, "DrawRectangle"),
    Napi::Function::New(env, BindDrawRectangle)
  );

  exports.Set(
    Napi::String::New(env, "DrawTexture"),
    Napi::Function::New(env, BindDrawTexture)
  );

  exports.Set(
    Napi::String::New(env, "DrawTexture"),
    Napi::Function::New(env, BindDrawTexture)
  );

	exports.Set(
    Napi::String::New(env, "DrawTextures"),
    Napi::Function::New(env, BindDrawTextures)
  );

	exports.Set(
    Napi::String::New(env, "AddArray"),
    Napi::Function::New(env, BindAddArray)
  );

	exports.Set(
    Napi::String::New(env, "DrawBunnies"),
    Napi::Function::New(env, BindDrawBunnies)
  );

  exports.Set(
    Napi::String::New(env, "DrawBunny"),
    Napi::Function::New(env, BindDrawBunny)
  );

  exports.Set(
    Napi::String::New(env, "DrawBunnyBuffer"),
    Napi::Function::New(env, BindDrawBunnyBuffer)
  );

  exports.Set(
    Napi::String::New(env, "DrawTextureFlat"),
    Napi::Function::New(env, BindDrawTextureFlat)
  );

  exports.Set(
    Napi::String::New(env, "DrawBunnyWhite"),
    Napi::Function::New(env, BindDrawBunnyWhite)
  );

  exports.Set(
    Napi::String::New(env, "DrawTextureFlatWhite"),
    Napi::Function::New(env, BindDrawTextureFlatWhite)
  );

  exports.Set(
    Napi::String::New(env, "DrawTextureShort"),
    Napi::Function::New(env, BindDrawTextureShort)
  );

  exports.Set(
    Napi::String::New(env, "DrawTextureShortWhite"),
    Napi::Function::New(env, BindDrawTextureShortWhite)
  );

  exports.Set(
    Napi::String::New(env, "DrawTextureShortBuffer"),
    Napi::Function::New(env, BindDrawTextureShortBuffer)
  );

  exports.Set(
    Napi::String::New(env, "DrawTextureInt"),
    Napi::Function::New(env, BindDrawTextureInt)
  );

  exports.Set(
    Napi::String::New(env, "GetColor"),
    Napi::Function::New(env, BindGetColor)
  );

  exports.Set(
    Napi::String::New(env, "GetTexturePointer"),
    Napi::Function::New(env, BindGetTexturePointer)
  );

  exports.Set(
    Napi::String::New(env, "DrawTexturePointer"),
    Napi::Function::New(env, BindDrawTexturePointer)
  );

  exports.Set(
    Napi::String::New(env, "DrawTexturePointerWhite"),
    Napi::Function::New(env, BindDrawTexturePointerWhite)
  );


  return exports;
}

NODE_API_MODULE(addon, Init);