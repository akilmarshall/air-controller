use macroquad::prelude::*;
use std::collections::HashMap;
use std::rc::Rc;

// sum type representing singleton or vectors of textures
enum Sum {
    Collection(Rc<Vec<Texture2D>>),
    Singleton(Rc<Texture2D>),
}
// structured database for system wide data
#[derive(Default)]
pub struct DB {
    textures: HashMap<String, Sum>,
}

impl DB {
    pub fn new() -> Self {
        DB {
            textures: HashMap::new(),
        }
    }
    pub async fn load_textures(&mut self) {
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
            .insert(String::from("plane"), Sum::Collection(Rc::new(plane)));
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
            .insert(String::from("digit"), Sum::Collection(Rc::new(digit)));
        // load odot texture
        self.textures.insert(
            String::from("odot"),
            Sum::Singleton(Rc::new(load_texture("resource/odot.png").await.unwrap())),
        );
        // load flag texture
        self.textures.insert(
            String::from("flag"),
            Sum::Singleton(Rc::new(load_texture("resource/flag.png").await.unwrap())),
        );
        // load bg texture
        self.textures.insert(
            String::from("ito"),
            Sum::Singleton(Rc::new(load_texture("resource/ito.png").await.unwrap())),
        );
    }
    pub fn planes(&self) -> Option<Rc<Vec<Texture2D>>> {
        if let Sum::Collection(planes) = &self.textures["plane"] {
            return Some(planes.clone());
        }
        None
    }
    pub fn digits(&self) -> Option<Rc<Vec<Texture2D>>> {
        if let Sum::Collection(digits) = &self.textures["digit"] {
            return Some(digits.clone());
        }
        None
    }
    pub fn odot(&self) -> Option<Rc<Texture2D>> {
        if let Sum::Singleton(odot) = &self.textures["odot"] {
            return Some(odot.clone());
        }
        None
    }
    pub fn flag(&self) -> Option<Rc<Texture2D>> {
        if let Sum::Singleton(flag) = &self.textures["flag"] {
            return Some(flag.clone());
        }
        None
    }
    pub fn bg(&self) -> Option<Rc<Texture2D>> {
        if let Some(bg) = self.textures.get("ito") {
            if let Sum::Singleton(t) = bg {
                return Some(t.clone());
            }
        }
        // if let Sum::Singleton(bg) = &self.textures["ito"] {
        //     return Some(bg.clone());
        // }
        None
    }
}
