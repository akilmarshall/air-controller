use macroquad::prelude::*;
use std::collections::HashMap;

// sum type represention singleton or vectors of textures
enum Sum {
    Collection(Vec<Texture2D>),
    Singleton(Texture2D),
}
// structured database for system wide data
struct DB {
    textures: HashMap<String, Sum>,
}

impl DB {
    async fn load_textures(&mut self) {
        // load plane textures
        let mut plane = Vec::new();
        for i in 0..11 {
            plane.push(
                load_texture(format!("resource/ship_{}.png", i).as_str())
                    .await
                    .unwrap(),
            );
        }
        self.textures
            .insert(String::from("plane"), Sum::Collection(plane));
        // load digit textures
        let mut digit = Vec::new();
        for i in 0..10 {
            digit.push(
                load_texture(format!("resource/digit_{}.png", i).as_str())
                    .await
                    .unwrap(),
            );
        }
        self.textures
            .insert(String::from("digit"), Sum::Collection(digit));
        // load odot texture
        self.textures.insert(
            String::from("odot"),
            Sum::Singleton(load_texture("resource/odot.png").await.unwrap()),
        );
        // load flag texture
        self.textures.insert(
            String::from("flag"),
            Sum::Singleton(load_texture("resource/flag.png").await.unwrap()),
        );
    }
}
