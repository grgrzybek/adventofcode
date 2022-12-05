/*
 * Copyright 2022 Grzegorz Grzybek
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

use std::env;
use std::fs::File;

pub struct Options {
    day: String,
    file: Option<File>
}

impl Options {
    pub fn solution(day: &str) -> Self {
        let template = Options {
            day: String::from(day),
            file: None
        };
        let args: Vec<String> = env::args().collect();
        if args.len() == 1 {
            println!("No file name specified");
            return template;
        }
        return match File::open(&args[1]) {
            Ok(file) => {
                Options {
                    file: Some(file),
                    ..template
                }
            },
            Err(_) => template
        }
    }

    pub fn day(&self) -> &str {
        &self.day
    }

    pub fn file(&self) -> Option<& File> {
        return match &self.file {
            Some(f) => Some(&f),
            None => None
        }
    }
}
