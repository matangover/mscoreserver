//=============================================================================
//  MuseScore
//  Music Composition & Notation
//  $Id:$
//
//  Copyright (C) 2013 Werner Schweer and others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENSE.GPL
//=============================================================================

#include "libmscore/score.h"
#include "libmscore/xml.h"
#include "libmscore/page.h"

#include "mscorecontroller.h"
using namespace Ms;

MScoreController::MScoreController()
      {
      }

MScoreController::~MScoreController()
      {
      }

//---------------------------------------------------------
//   processPng
//---------------------------------------------------------

static void processPng(Score* score)
      {
      qreal convDpi = 100.0;
      qreal mscoreDpi = 300.0;
      score->setPrinting(true);

      QImage::Format f = QImage::Format_ARGB32_Premultiplied;

      const QList<Page*>& pl = score->pages();

      Page* page = pl.at(0);

      QRectF r = page->abbox();
      int w = lrint(r.width()  * convDpi / mscoreDpi);
      int h = lrint(r.height() * convDpi / mscoreDpi);

      QImage printer(w, h, f);
      printer.setDotsPerMeterX(lrint((convDpi * 1000) / INCH));
      printer.setDotsPerMeterY(lrint((convDpi * 1000) / INCH));

      printer.fill(0xffffffff);

      double mag = convDpi / mscoreDpi;
      QPainter p(&printer);

      p.setRenderHint(QPainter::Antialiasing, true);
      p.setRenderHint(QPainter::TextAntialiasing, true);
      p.scale(mag, mag);

      foreach(const Element* e, page->elements()) {
            if (!e->visible())
                  continue;
            QPointF pos(e->pagePos());
            p.translate(pos);
            e->draw(&p);
            p.translate(-pos);
            }
      score->setPrinting(false);
      QByteArray ba;
      QBuffer buffer(&ba);
      buffer.open(QIODevice::WriteOnly);
      printer.save(&buffer, "PNG");
      // response.setHeader("Content-Type", "image/png");
      // response.write(ba, true);
      QFile out("/Users/matan/Documents/code/mscoreserver/output.png");
      if (!out.open(QIODevice::WriteOnly)) {
            printf("Couldn't open file");
            return;
      }
      out.write(ba);
      }

//---------------------------------------------------------
//   processJpeg
//---------------------------------------------------------

// static void processJpeg(HttpResponse& response, Score* score)
//       {
//       qreal convDpi = 100.0;
//       qreal mscoreDpi = 300.0;
//       score->setPrinting(true);

//       QImage::Format f = QImage::Format_ARGB32_Premultiplied;

//       const QList<Page*>& pl = score->pages();

//       Page* page = pl.at(0);

//       QRectF r = page->abbox();
//       int w = lrint(r.width()  * convDpi / mscoreDpi);
//       int h = lrint(r.height() * convDpi / mscoreDpi);

//       QImage printer(w, h, f);
//       printer.setDotsPerMeterX(lrint((convDpi * 1000) / INCH));
//       printer.setDotsPerMeterY(lrint((convDpi * 1000) / INCH));

//       printer.fill(0xffffffff);

//       double mag = convDpi / mscoreDpi;
//       QPainter p(&printer);

//       p.setRenderHint(QPainter::Antialiasing, true);
//       p.setRenderHint(QPainter::TextAntialiasing, true);
//       p.scale(mag, mag);

//       foreach(const Element* e, page->elements()) {
//             if (!e->visible())
//                   continue;
//             QPointF pos(e->pagePos());
//             p.translate(pos);
//             e->draw(&p);
//             p.translate(-pos);
//             }
//       score->setPrinting(false);
//       QByteArray ba;
//       QBuffer buffer(&ba);
//       buffer.open(QIODevice::WriteOnly);
//       if (!printer.save(&buffer, "JPEG"))
//             printf("   export failed\n");
//       response.setHeader("Content-Type", "image/jpeg");
//       response.write(ba, true);
//       }

// //---------------------------------------------------------
// //   processGif
// //---------------------------------------------------------

// static void processGif(HttpResponse& response, Score* score)
//       {
//       qreal mscoreDpi = 300.0;
//       printf("process gif...\n");
//       qreal convDpi = 100.0;
//       score->setPrinting(true);

//       QImage::Format f = QImage::Format_ARGB32_Premultiplied;

//       const QList<Page*>& pl = score->pages();

//       Page* page = pl.at(0);

//       QRectF r = page->abbox();
//       int w = lrint(r.width()  * convDpi / mscoreDpi);
//       int h = lrint(r.height() * convDpi / mscoreDpi);

//       QImage printer(w, h, f);
//       printer.setDotsPerMeterX(lrint((convDpi * 1000) / INCH));
//       printer.setDotsPerMeterY(lrint((convDpi * 1000) / INCH));

//       printer.fill(0xffffffff);


//       double mag = convDpi / mscoreDpi;
//       QPainter p(&printer);

//       p.setRenderHint(QPainter::Antialiasing, true);
//       p.setRenderHint(QPainter::TextAntialiasing, true);
//       p.scale(mag, mag);

//       foreach(const Element* e, page->elements()) {
//             if (!e->visible())
//                   continue;
//             QPointF pos(e->pagePos());
//             p.translate(pos);
//             e->draw(&p);
//             p.translate(-pos);
//             }
//       score->setPrinting(false);
//       QByteArray ba;
//       QBuffer buffer(&ba);
//       buffer.open(QIODevice::WriteOnly);
//       if (!printer.save(&buffer, "GIF"))
//             printf("   export failed\n");
//       response.setHeader("Content-Type", "image/gif");
//       printf("   respond %d\n", ba.size());
//       response.write(ba, true);
//       }

//---------------------------------------------------------
//   service
//---------------------------------------------------------

void MScoreController::service()
      {
      QFile f("/Users/matan/Documents/code/mscoreserver/share/templates/03-Chamber_Music/03-Wind_Quintet.mscx");
      if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            printf("Couldn't open file");
            return;
      }

      MasterScore* score = new MasterScore(MScore::defaultStyle());
      // if (name.endsWith(".mscx")) {
            QByteArray data = f.readAll();
            XmlReader xr(data);
            score->read1(xr, true);
            // }
      // else if (name.endsWith(".mscz")) {
      //       score->loadCompressedMsc(f, true);
      //       }
      // else {
      //       printf("bad file type <%s>\n", qPrintable(name));
      //       return;
      //       }

      // QByteArray s = request.getParameter("size");
      // if (!s.isEmpty()) {
            // int size = s.toInt();
            // int size = 200;
            // if (size < 10)
            //       size = 10;
            // else if (size > 400)
            //       size = 400;
            // qreal spatium = score->spatium();
            // spatium = (spatium * size) / 100.0;
            // score->setSpatium(spatium);
            // }
      // s = request.getParameter("transpose");
//             if (!s.isEmpty()) {
//                   int transpose = s.toInt();
//                   printf("transpose %d semitones\n", transpose);

//                   score->doLayout();
//                   score->cmdSelectAll();
//                   TransposeDirection d = transpose < 0 ? TRANSPOSE_DOWN :TRANSPOSE_UP;
//                   if (transpose < 0)
//                         transpose = -transpose;
//                   score->transpose(TRANSPOSE_BY_INTERVAL,
//                      d, 0, transpose, true, true, false);
// //                  score->deselectAll();
//                   }

      score->doLayout();
      //
      // process score
      //
      // QByteArray format = request.getParameter("format");
      // if (format == "png")
            processPng(score);
      // else if (format == "jpeg")
            // processGif(response, score);
      // else if (format == "gif")
            // processGif(response, score);
      // else {
            // printf("bad format <%s>\n", qPrintable(format));
            // return;
            // }
      delete score;
      }
