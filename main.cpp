#include <iostream>
#include <vector>
#include <genie/resource/PalFile.h>
#include <fstream>
#include <genie/resource/SlpFile.h>
#include <genie/resource/SlpFrame.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Texture.hpp> 
#include "genie/resource/DrsFile.h" 
#include "genie/resource/SlpFile.h" 
#include "genie/resource/Color.h"
#include <genie/script/ScnFile.h>
#include <genie/file/Compressor.h>
#include <genie/lang/LangFile.h>
#include <boost/iostreams/copy.hpp>
#include <boost/smart_ptr.hpp>

sf::Image createSfImage(uint32_t width, uint32_t height, const uint8_t* pixels, 
                        genie::PalFilePtr palette, uint8_t transparent_pixel)
{
  sf::Image img;
  
  img.create(width, height, sf::Color::Transparent);
  
  for (uint32_t row=0; row < height; row++)
    for (uint32_t col=0; col < width; col++)
    {
      uint8_t c_index = pixels[row * width + col];
      
      if (c_index != transparent_pixel)
      {
        genie::Color g_color = (*palette)[c_index];
        img.setPixel(col, row, sf::Color(g_color.r, g_color.g, g_color.b));
      }           
    }
    
  return img;
}

sf::Image createSfImage(genie::SlpFramePtr frame, genie::PalFilePtr palette)
{
  return createSfImage(frame->getWidth(), frame->getHeight(), 
                       frame->getPixelIndexes(), palette, 
                       frame->getTransparentPixelIndex());
}


void testDrs()
{
  genie::DrsFile interfac;
  
  interfac.setGameVersion(genie::GV_AoK);
  interfac.load("slptest/interfac.drs");
  
  genie::PalFilePtr pal = interfac.getPalFile(50500);
  
  genie::DrsFile drs;
  
//   drs.setGameVersion(genie::GV_AoE);
//   drs.load("slptest/graphics_aoe.drs");
  
//   drs.setGameVersion(genie::GV_SWGB);
//   drs.load("slptest/graphics_x1.drs");
  
  drs.setGameVersion(genie::GV_AoK);
//   drs.load("slptest/graphics.drs");
  drs.load("slptest/terrain.drs");
  
  sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML Graphics");

  uint32_t slp_id = 15001;//1057 ;

  
  sf::Texture text, text2;
    
  genie::SlpFilePtr slpf = drs.getSlpFile(slp_id);
  drs.getSlpFile(slp_id);
    
  sf::Image img;
  img.loadFromFile("image_part.bmp");
  
  text.loadFromImage(createSfImage(slpf->getFrame(1), pal));
  text2.loadFromImage(img);
  
  
  sf::Sprite Sprite(text);
  sf::Sprite Sprite2(text2);

  Sprite.setPosition(0, 0);
  Sprite2.setPosition(100,50);

  // Start game loop
  while (App.isOpen())
  {
      // Process events
      sf::Event Event;
      while (App.pollEvent(Event))
      {
          // Close window : exit
          if (Event.type == sf::Event::Closed)
              App.close();
          
      }

      // Clear screen
      App.clear();

      // Display sprite in our window
      App.draw(Sprite);
      App.draw(Sprite2);

      // Display window contents on screen
      App.display();
  }
}

void testScn()
{
  genie::ScnFile scn;
  
  scn.extractRaw("testmap_small_random.scx", "raw.scx");
//   scn.extractRaw("test2.scx", "raw.scx");
//   scn.extractRaw("Gwyndlegard - Multiplayer Edition 1,0.scx", "raw.scx");
   
  scn.load("Nomadic Asia by Wolfy.scx");
//   scn.load("testmap_small_random.scx");
//   scn.load("Gwyndlegard - Multiplayer Edition 1,0.scx");
  
  std::cout << "Original filename: " << scn.originalFileName << std::endl;
  
//   std::cout << scn.scenarioInstructions << std::endl;
//   std::cout << scn.resource.instructions << std::endl;
//   std::cout << scn.resource.scouts << std::endl;
//   std::cout << scn.resource.bitmapIncluded << std::endl;
  std::cout << "Bitmap byte size: " << scn.resource.bitmapByteSize << std::endl;
  
  sf::Image img;
  
  //std::cout << "AI Name 0: " << scn.playerData2.aiNames[0] << std::endl;
  //std::cout << "AI Name 15: " << scn.playerData2.aiNames[15] << std::endl;
  
  //std::cout << scn.map.width << "x" << scn.map.height << std::endl;
  
  /*
  std::ofstream ofs;
  ofs.open("qqq", std::ios::binary);
  
  ofs.write(scn.resource.bitmap, scn.resource.bitmapByteSize);
  ofs.close();
  */
  
  
//   scn.saveAs("xxxx.scx");
  
//   genie::ScnFile scn2;
//   scn2.load("xxxx.scx");
  
  
//   std::cout << "Original filename: " << scn2.originalFileName << std::endl;
  
}

void testLang()
{
  genie::LangFile lf;
  
  lf.load("language.dll");
  
  std::cout << "GetString: " << lf.getString(42320) << std::endl;
  
  lf.setString(42320, "Test pcrio ftw! U: ö ü ä ÜÄÖ haha");
  
  std::cout << "GetOtherString: " << lf.getString(4442) << std::endl;
  
  lf.saveAs("new_lang.dll");
  
  lf.load("new_lang.dll");
  
  
  std::cout << "GetSaved String: \"" << lf.getString(42320) << "\"" << std::endl;
  
  lf.setString(1, "new one");
  std::cout << lf.getString(1) << std::endl;

  std::cout << "Loading aoe lang: " << std::endl;
  lf.load("lang/aoe/language.dll");
  lf.getString(54518);
  
  genie::LangFile *lptr = new genie::LangFile();
  
  lptr->load("lang/sw/language.dll");
  lptr->getString(3064);
  
  delete lptr;
  
}

int main(int argc, char **argv) {

  genie::Logger::setLogLevel(genie::Logger::L_INFO);
  
  std::ofstream log_out;
  log_out.open("log.txt");
  
  genie::Logger::setGlobalOutputStream(log_out);
  
//   testDrs();
  
//   testScn();
  testLang();
  
  /*
  std::ifstream ifs;
  std::ofstream ofs;
  
  ifs.open("tmp/empires2.dat", std::ios::binary);
  ofs.open("q.dat", std::ios::binary);
 
  genie::Compressor c;
  
  boost::shared_ptr<std::istream> unc = c.startDecompression(&ifs);
  
  boost::iostreams::copy((*unc), ofs);
  
  c.stopDecompression();
  
//   delete unc;
  
  ifs.close();
  ofs.close();
   */
  
  return 0;
}
