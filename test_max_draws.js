const r = require('./build/Release/ralts.node')
const {Worker} = require('worker_threads')

const screenWidth = 1280
const screenHeight = 720



class Bunny {

	constructor() {
			this.pos = {
					x: Math.random() * screenWidth,
					y: Math.random() * screenHeight
			}

			this.speed = {
					x: (-250+Math.random() * 500)/60,
					y: (-250+Math.random() * 500)/60
			}

			this.color = Color(
					80 + Math.random() * 170,
					80 + Math.random() * 170,
					80 + Math.random() * 170,
					255
			)

			this.color_buffer = Buffer.from([this.color.r, this.color.g, this.color.b, this.color.a])
	}

	update() {
			this.pos.x += this.speed.x
			this.pos.y += this.speed.y

			if (this.pos.x > screenWidth || this.pos.x < 0)
					this.speed.x = -this.speed.x

			if (this.pos.y > screenHeight || this.pos.y < 0)
					this.speed.y = -this.speed.y
	}

}

function Color(r, g, b, a) {
	return {
		r: r,
		g: g,
		b: b,
		a: a
	}
}

let white = Color(255, 255, 255, 255)

let lightgray = Color(128, 128, 128, 255)

let black = Color(0, 0, 0, 255)

let green = Color(0, 255, 0, 255)

r.InitWindow(screenWidth, screenHeight, "Hello World")
console.clear()
for (let key in r) {
	console.log(key)
	console.log(r[key])
}
console.log(r)
let draw_methods = [

	{
		name: 'DrawTexture',
		args: 4,
		func: bunny => {
			r.DrawTexture(bunny_texture, bunny.pos.x, bunny.pos.y, bunny.color)
		},
		bunnies: 0
	},

	{
		name: 'DrawTextureShortInt',
		args: 6,
		func: bunny => {
			let c = bunny.color
			var r = c.r & 0xFF;
			var g = c.g & 0xFF;
			var b = c.b & 0xFF;
			var a = c.a & 0xFF;
			var rgb = (r << 24) + (g << 16) + (b << 8) + (a)

			//r.DrawTextureInt(bunny_texture.id, bunny_texture.width, bunny_texture.height, bunny.pos.x, bunny.pos.y, rgb)
		},
		bunnies: 0
	},

	{
		name: 'DrawTextureShortWhite',
		args: 5,
		func: bunny => {
			r.DrawTextureShortWhite(bunny_texture.id, bunny_texture.width, bunny_texture.height, bunny.pos.x, bunny.pos.y)
		},
		bunnies: 0
	},

	{
		name: 'DrawTextureShort',
		args: 9,
		func: bunny => {
			let c = bunny.color
			r.DrawTextureShort(bunny_texture.id, bunny_texture.width, bunny_texture.height, bunny.pos.x, bunny.pos.y, c.r, c.g, c.b, c.a)
		},
		bunnies: 0
	},

	{
		name: 'DrawBunnyWhite',
		args: 2,
		func: bunny => {
			r.DrawBunnyWhite(bunny.pos.x, bunny.pos.y)
		},
		bunnies: 0
	},

	{
		name: 'DrawBunny',
		args: 6,
		func: bunny => {
			let c = bunny.color
			r.DrawBunny(bunny.pos.x, bunny.pos.y, c.r, c.g, c.b, c.a)
		},
		bunnies: 0
	},

	{
		name: 'DrawBunnyBuffer',
		args: 1,
		func: bunny => {
			let c = bunny.color
			let buf = Buffer.from([bunny.pos.x, bunny.pos.y, c.r, c.g, c.b, c.a])
			r.DrawBunnyBuffer(buf)
		},
		bunnies: 0
	},

	{
		name: 'DrawTextureFlat',
		args: 11,
		func: bunny => {
			let c = bunny.color
			let t = bunny_texture
			r.DrawTextureFlat(
				t.id, t.width, t.height, t.mipmaps, t.format,
				bunny.pos.x, bunny.pos.y,
				c.r, c.g, c.b, c.a
			)
		},
		bunnies: 0
	},

	{
		name: 'DrawTextureFlatWhite',
		args: 7,
		func: bunny => {
			let t = bunny_texture
			r.DrawTextureFlatWhite(
				t.id, t.width, t.height, t.mipmaps, t.format,
				bunny.pos.x, bunny.pos.y
			)
		},
		bunnies: 0
	},

	{
		name: 'DrawTextureShortBuffer',
		args: 6,
		func: bunny => {
			//let c = bunny.color
			let t = bunny_texture
			r.DrawTextureShortBuffer(
				t.id, t.width, t.height,
				bunny.pos.x, bunny.pos.y,
				bunny.color_buffer
			)
		},
		bunnies: 0
	}

]

let bunny_texture = r.LoadTexture('sprite.png')
let bunnies = []

let method = 0
let quit = false

while (!r.WindowShouldClose() && !quit) {
	r.BeginDrawing()
	r.ClearBackground(white)

	if (r.GetFPS() > 60 || bunnies.length < 1000) {
		for (let i = 0; i < 10; i++) {
			bunnies.push(new Bunny())
		}
	} else {
		draw_methods[method].bunnies = bunnies.length
		bunnies = []
		method += 1
		if (method == draw_methods.length) {
			quit = true
			break
		}
	}

	for (let bunny of bunnies) {
		bunny.update()
		if (draw_methods[method]) {
			draw_methods[method].func(bunny)
		}
	}
	
	r.DrawRectangle(0, 0, screenWidth, 40, black)
	r.DrawText(`method: ${draw_methods[method].name}		` + "bunnies: "+ bunnies.length, 10, 10, 20, green);
	r.DrawFPS(1200, 10)

	r.EndDrawing()
}

for (let method of draw_methods) {
	console.log(`method: ${method.name}, args: ${method.args}, bunnies: ${method.bunnies}`)
}