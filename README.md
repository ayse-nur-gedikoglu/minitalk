# minitalk
**Bu projenin amacı *UNIX sinyallerini* kullanarak küçük bir veri değişim programı kodlamaktır .**

**İzin Verilen Fonksiyonlar**

| Fonksiyon | Başlık Dosyası(ları) | Açıklama |
| --- | --- | --- |
| `malloc` | `stdlib.h` | Dinamik bellek ayırır. |
| `free` | `stdlib.h` | Ayrılan belleği serbest bırakır. |
| `write` | `unistd.h` | Dosya tanımlayıcısına veri yazar. |
| `getpid` | `unistd.h` | Süreç kimliğini döndürür. |
| `signal` | `signal.h` | Sinyal işleyicisi atar (eski yöntem). |
| `sigemptyset` | `signal.h` | Sinyal kümesini başlatır (boşaltır). |
| `sigaddset` | `signal.h` | Sinyal kümesine sinyal ekler. |
| `sigaction` | `signal.h` | Gelişmiş sinyal işleme. |
| `pause` | `unistd.h` | Sinyal bekler. |
| `kill` | `sys/types.h`, `signal.h` | Sürece sinyal gönderir. |
| `sleep` | `unistd.h` | Belirtilen saniye kadar bekler. |
| `usleep` | `unistd.h` | Belirtilen mikro saniye kadar bekler (POSIX'te olmayabilir). |
| `exit` | `stdlib.h` | Süreci sonlandırır. |

bu fonksiyonlara biraz daha detaylı bakacak olursak;

**Süreç Yönetimi:**

- **`getpid`**: Çağıran sürecin süreç kimliğini (process ID - PID) döndürür. Her sürecin benzersiz bir PID'si vardır. Örneğin: `pid_t pid = getpid();` geçerli sürecin PID'sini `pid` değişkenine atar.

## pid_t nedir?

`pid_t`  Unix ve Linux'ta muhtemelen işaretsiz bir int olan süreç tanımlayıcısının türüdür. Bir ps (süreç durumu) komutunun ilk sütununda görünen sayıdır:
`1. $ ps 
2.       PID    PPID    PGID       TTY         UID    STIME COMMAND 
3.      9252     772    9252      pty0      197609 14:02:00 /usr/bin/ps 
4. S    8984     772    8984      pty0      197609   Oct 22 /usr/bin/vi 
5.       772    9232     772      pty0      197609   Oct 22 /usr/bin/bash 
6.      9232       1    9232      ?         197609   Oct 22 /usr/bin/mintty`

İkinci sütun ebeveynin işlem kimliğidir. Zinciri görebiliriz: mintty sistem tarafından başlatıldı, burada Cygwin, işlem kimliği 1. Mintty, hem vi hem de ps'yi başlatan bash işlemini başlattı.

Yeni süreçler sistem çağrısı veya onun varyantlarından biriyle oluşturulur `pid=fork()`. Fork aslında çalışan bir süreci kopyalar. Ana süreç olarak adlandırılan orijinal sürece fork, asla sıfır olmayan süreç kimliğini döndürür. Yeni süreç olan çocuğa fork, 0 döndürür.

## Neden Procces Id mizi int değil de Pid_t veri türünde tanımlarız? İnt tanımlamamız ne gibi sonuçlar doğuracaktır.

### **Derleyici ve Veri Türleri Uyumu**

Bir değişkenin veri türü (örneğin `int`, `long`) hafızada ne kadar yer kaplayacağına ve işlemci düzeyinde nasıl işleneceğine karar verir. İşletim sistemi veya donanım mimarisi farklı olduğunda bu türlerin kapladığı alan ve işlemler değişebilir.

- 32-bit sistemlerde `int` genellikle **4 bayt** (32 bit) yer kaplar.
- 64-bit sistemlerde bazı veri türleri (örneğin `long`) daha geniştir ve **8 bayt** (64 bit) olabilir.

**Örnek:** Eğer işletim sisteminiz işlem kimlikleri için daha büyük değerler destekliyorsa, `pid_t` doğrudan `long` olarak tanımlanabilir.

### **pid_t'nin Soyutlanmasının Faydası**

`pid_t` ile işlem kimlikleri saklandığında:

- İşletim sistemi ve derleyici, bu türün ne olması gerektiğini sizin yerinize belirler.
- Eğer sistem `int` yerine `long` kullanıyorsa, derleyici `pid_t`'yi otomatik olarak `long` ile eşler ve kodunuz hatasız çalışır.

> Siz pid_t kullandığınızda bu detayları düşünmek zorunda kalmazsınız çünkü POSIX başlık dosyaları (sys/types.h) bunu sizin yerinize yapar.
> 

**Doğru Veri Tipiyle İşlem:** Eğer sistem PID için daha büyük türler gerektiriyorsa (örneğin büyük sunucularda), `pid_t` türü otomatik olarak uygun veri tipiyle eşleştirilir ve kodun taşınabilirliği sağlanır.

### **Farklı Platformlardaki PID Sınırları**

İşlem kimlikleri (PID'ler) işletim sistemine göre farklı maksimum değerler alabilir:

- **Linux:** PID genellikle `int` türünde ve 32-bit'tir (maksimum değeri 2,147,483,647).
- **Eski UNIX Sistemleri:** PID'ler daha küçük ve 16-bit değerlerle sınırlı olabilir (maksimum değeri 32,767).
- **Gömülü Sistemler:** PID türü bellek kısıtlamaları nedeniyle daha küçük `short` türünde olabilir.

Eğer doğrudan `int` kullansaydınız, daha küçük sistemlerde taşma (overflow) veya yanlış işlem kimlikleri gibi sorunlar yaşanabilirdi.

## Bir Program Bittiğinde Process İd ye Ne Olur?

### **Nasıl Oluyor?**

1. **`main` fonksiyonu bittiğinde**
    - `return` veya `exit()` ile çıkıldığında, program tamamen sona erer ve process de **sonlanır**.
2. **Sistem, process'in kaynaklarını temizler**
    - İşletim sistemi, process’in kullandığı **bellek, dosya tanıtıcıları (file descriptors) ve diğer kaynakları** serbest bırakır.
    - **Ancak!** Eğer bir **çocuk process (child process)** varsa ve ebeveyn (parent) onu beklememişse, **zombi process** olarak kalabilir.

## SIGUSR1 VE SIGUSR2 NEDİR? Neden kullanıcı kullanımına açıktırlar, nerede kullanılırlar?

**SIGUSR1** ve **SIGUSR2** sinyalleri POSIX standardında tanımlıdır ve genellikle birçok UNIX tabanlı sistemde (Linux dahil) kullanılırlar. Ancak bu sinyaller **kullanıcı tanımlı sinyaller** olarak geçer ve belirli bir varsayılan işlemle ilişkili değildirler. Diğer sinyallerin aksine sistem tarafından belirlenmiş bir işlevleri bulunmaz.

---

### **Neden Varsayılan İşlev Tanımlı Değildir?**

1. **Kullanıcı Tanımlı Sinyaller:**
    
    SIGUSR1 ve SIGUSR2, kullanıcı uygulamaları tarafından belirli bir işlevle ilişkilendirilmek üzere tasarlanmıştır. İşletim sistemi bu sinyalleri belirli bir varsayılan davranışla bağlamaz çünkü kullanıcıların bunları esnek bir şekilde kullanması amaçlanmıştır.
    
2. **Genel Amaçlı Kullanım:**
    
    Sistem sinyalleri genellikle belirli olaylara atanmıştır (örneğin, SIGTERM = işlem sonlandırma, SIGSEGV = bellek ihlali). Ancak SIGUSR1 ve SIGUSR2 bu tür sınırlamalara tabi tutulmaz. Uygulamanın ihtiyacına göre anlamlandırılabilir.
    
3. **Sistemden Bağımsızlık:**
    
    Sistem sinyalleri genellikle işletim sisteminin veya çekirdeğin belirli işlevleriyle ilişkilidir (örneğin, SIGKILL işlem öldürme sinyalidir ve bu sinyal durdurulamaz). Kullanıcı tanımlı sinyaller ise sistemle doğrudan bir ilişkiye sahip olmadığından uygulama düzeyinde daha özgürce kullanılabilir.
    

---

### **Ne Zaman Kullanılırlar?**

SIGUSR1 ve SIGUSR2 genellikle şu senaryolarda tercih edilir:

- **IPC (Process Communication):** Birden fazla işlem arasında özel iletişim sağlamak için.
- **Debugging:** Belirli bir kod yolunu tetiklemek veya hata ayıklamak amacıyla sinyal gönderimi.
- **Özelleştirilmiş Davranış:** Belirli bir sinyalle işlem durumunu güncellemek veya özel görevleri başlatmak.

## Sinyal İşleme Süreci Nasıl İlerler?

Sinyalin işlenme süreci, **işletim sistemi (kernel)** ve **işlemler** arasındaki etkileşimi içerir. Bu süreçte çekirdek (kernel) önemli bir rol oynar, ancak işlemler de sinyalleri alır ve işleyebilir. İşte genel bir **sinyalin işlenme süreci**:

### 1. **Sinyalin Gönderilmesi**:

Sinyal, genellikle aşağıdaki gibi bir durumda gönderilir:

- Bir işlem, başka bir işlem veya işlem grubu ile iletişim kurmak amacıyla sinyal gönderir.
- Sinyal, terminalden gelen bir komutla (örneğin `kill` komutu) ya da program içerisindeki `kill()`, `signal()`, `raise()` gibi fonksiyonlarla gönderilebilir.

Örnek komut:

```bash
kill -9 <PID>
```

Bu komut, belirtilen PID'ye (işlem kimliği) `SIGKILL` (sinyali 9) gönderir.

### 2. **Sinyalin Kernel Tarafından İşlenmesi**:

Kernel, **sinyalin gönderileceği işlem** hakkında bilgi alır. Bu işlem **PID** (işlem kimliği) aracılığıyla hedeflenir.

- Eğer sinyal **yönlendirilmişse** (örneğin, belirli bir PID'ye), kernel, hedef işlemi belirler.
- Eğer sinyal **işlem grubuna** gönderiliyorsa, kernel tüm grup üyelerine sinyali iletir.

### 3. **Kernel'in Sinyali İşlemesi**:

Kernel, sinyalin türüne göre belirli bir işlem yapar:

- **İşlem durdurulmazsa**: Eğer sinyal **işlem tarafından işlenebiliyorsa** (örneğin, `SIGINT`, `SIGTERM` gibi), kernel, işlem üzerinde sinyalin nasıl işleneceğini belirler. Örneğin, `SIGINT` sinyali genellikle kullanıcı tarafından `Ctrl+C` ile gönderilir ve işlem **bitirilmeden önce temizlenebilir** (cleanup işlemleri yapılabilir).
- **İşlem sonlandırılırsa**: Eğer sinyal **sonlandırıcı türde bir sinyalse** (örneğin `SIGKILL`), kernel işlem üzerinde **sonlandırma** işlemi başlatır. `SIGKILL`, işlem tarafından engellenemez, kernel hemen işlemi sonlandırır.

### 4. **İşlem Sinyali Aldığında**:

Bir işlem, **signal handler** veya **default action** aracılığıyla sinyali alabilir. Bir işlem bu sinyali işlemek için şu yolları izler:

- **Signal Handler**: Eğer işlem, belirli bir sinyal için bir **signal handler** (sinir işleyici fonksiyon) tanımlamışsa, kernel sinyal geldiğinde bu handler fonksiyonunu çağırır. Örneğin:
    
    ```c
    void signal_handler(int signum) {
        if (signum == SIGINT) {
            // İşlemle ilgili bazı işlemler yapılır.
        }
    }
    
    signal(SIGINT, signal_handler);
    
    ```
    
    Bu durumda, işlem `SIGINT` sinyali aldığında `signal_handler` fonksiyonu çalıştırılır.
    
- **Default Action**: Eğer işlem için bir handler tanımlanmadıysa, kernel **varsayılan bir işlem** yapar. Örneğin, `SIGKILL` sinyali alındığında, işlem derhal sonlandırılır.

### 5. **İşlem ve Kernel İletişimi**:

Kernel, işlemi yönlendirdiği sinyali işlemi sonlandırmaya veya ilgili sinyalin işlenmesine yönlendirir:

- **Sinyalin Sonuçları**:
    - Eğer **işlem sonlandırma sinyali (kill)** ise: Kernel işlem üzerinde sonlandırma işlemi başlatır ve işlem durur.
    - Eğer **diğer sinyaller (pause, SIGUSR1, SIGUSR2, SIGTERM vb.)** ise, işlem bu sinyalleri işler ve uygun işlemleri yapar.

### 6. **Sinyalin Alındığı Durumlar**:

- **`SIGKILL`**: İşlem tarafından engellenemez, kernel derhal işlemi sonlandırır.
- **`SIGTERM`**: İşlem sonlandırılabilir, ancak işlem kendisini engelleyebilir veya önceden tanımlı bir işleyici ile bu sinyali özelleştirebilir.
- **`SIGINT`**: Kullanıcı tarafından genellikle `Ctrl+C` ile gönderilir, işlem sonlandırılabilir.
- **`SIGUSR1` ve `SIGUSR2`**: Kullanıcı tanımlı sinyallerdir, işlem bu sinyallerle iletişim kurabilir.

### 7. **Sinyalin Sonuçları**:

- Eğer işlem **sonlandırıldıysa**, kernel işlem tablosundan çıkarılır ve kaynaklar serbest bırakılır.
- Eğer işlem **devam ediyorsa**, sinyal işlenir ve işlem kontrolü devam eder.

---

### **Özetle**:

1. **Sinyal Gönderimi**: Bir işlem (veya terminal, kullanıcı) sinyal gönderir.
2. **Kernel'in Rolü**: Kernel, sinyali işleme koyar ve uygun hedef işlem veya işlem grubuna yönlendirir.
3. **İşlem Tarafından İşlenmesi**: İşlem, sinyali ya `signal handler` ile işler ya da kernel'in belirlediği varsayılan eylemle sinyali işler.
4. **Sonuçlar**: İşlem, sinyali aldıktan sonra belirli işlemler yapar (örneğin sonlandırma, devam etme, yanıt verme).

Bu süreç, temel olarak sinyalin gönderilmesinden, kernel ve işlem arasındaki etkileşime kadar devam eder.

*Hap Bilgi :*

> *ppıd ne zaman aynı olur?*
> 

`*ppid` (Parent Process ID), bir sürecin ebeveyninin (parent process) kimliğini temsil eder.*

**`*ppid` ne zaman aynı olur?***

- ***Aynı ebeveyn tarafından oluşturulan süreçler**: Eğer iki süreç aynı ebeveyn tarafından oluşturulmuşsa, bu süreçlerin `ppid`'leri aynı olacaktır. Örneğin, bir programda `fork()` çağrısıyla birden fazla çocuk süreci oluşturulursa, hepsinin `ppid` değeri aynı olur, çünkü aynı ebeveynden (parent) türemişlerdir.*
- ***Orphan süreçler**: Eğer bir süreç, ebeveyn süreci sonlanırsa (terminate), bu süreç **yetim süreç (orphan process)** haline gelir ve sistem tarafından `init` sürecine (Linux'ta `pid=1`) atanır. Bu durumda, bu süreçlerin hepsi için `ppid=1` olur.*

Ebeveyn (veya uygun kimlik bilgilerine sahip, işlem kimliğine sahip herhangi bir işlem), bunu çocuğu kontrol etmek için kullanabilir. Genellikle ebeveyn sadece yapar `wait(…)`veya `waitid(pid, …)`çocuk bitirene kadar orada oturur. Çocuğu kullanarak bir sinyal gönderebilir `kill(pid, signum)`veya bir hata ayıklayıcı gibi davranabilir ve çocuğun durumunu kullanarak inceleyebilir `ptrace(pid, …)`, işlem kimliği için birkaç kullanımdan bahsetmek gerekirse.

**Sinyal İşleme:**

Sinyaller, işletim sisteminin süreçlere gönderdiği bildirimlerdir. Örneğin, Ctrl+C tuşlarına basmak bir `SIGINT` sinyali gönderir.

- **`signal` (Eski Yöntem)**: Bir sinyal için bir sinyal işleyici (signal handler) atamak için kullanılan eski bir yöntemdir. Ancak, bazı dezavantajları nedeniyle (özellikle güvenilirlik ve taşınabilirlik açısından) `sigaction` tercih edilir.
- **`sigemptyset` & `sigaddset`**:
    - **`sigemptyset`**: Bir sinyal kümesini başlatmak için kullanılır. Kümedeki tüm sinyalleri temizler.
    - **`sigaddset`**: Bir sinyali bir sinyal kümesine eklemek için kullanılır. Bu fonksiyonlar genellikle `sigaction` ile birlikte kullanılır.
- **`sigaction`**: Sinyal işleme için daha gelişmiş ve güvenilir bir arayüz sağlar. Bir sinyal için bir işleyici atamanın yanı sıra, sinyal maskelerini ve diğer davranışları da kontrol etmenizi sağlar.
- **`pause`**: Bir sinyal gelene kadar süreci askıya alır. Bir sinyal alındığında, `pause` fonksiyonu geri döner.
- **`kill`**: Bir sürece bir sinyal göndermek için kullanılır. Örneğin: `kill(pid, SIGTERM);` belirtilen PID'ye sahip sürece `SIGTERM` sinyali gönderir (genellikle süreci sonlandırmak için kullanılır).

**Zaman Yönetimi:**

- **`sleep`**: Belirtilen sayıda saniye boyunca süreci askıya alır.
- **`usleep`**: `sleep` fonksiyonuna benzer, ancak mikron saniye cinsinden askıya alma sağlar.

**Çıkış:**

- **`exit`**: Bir süreci sonlandırmak için kullanılır. Bir çıkış kodu alır. 0 genellikle başarılı bir çıkışı, 0'dan farklı bir değer ise bir hatayı gösterir.

*Daha fazla bilgi için -KESİNLİKELE!!- bunu oku: https://www.codequoi.com/en/creating-and-killing-child-processes-in-c/*

---

## SİNYALLER

`signal()` fonksiyonu, bir C programında **sinyal işleme** (signal handling) mekanizmasını kurmak için kullanılan bir fonksiyondur. Sinyaller, işletim sistemi tarafından bir sürece belirli olayları bildirmek için kullanılan bir yöntemdir. Örneğin, bir kullanıcı `Ctrl+C` tuşlarına bastığında, çalışmakta olan programa `SIGINT` sinyali gönderilir.

---

### **`signal()` Fonksiyonunun Prototipi:**

```c
c
KopyalaDüzenle
#include <signal.h>void (*signal(int signum, void (*handler)(int)))(int);

```

- **`signum`**: İşlem sırasında yakalanması istenen sinyalin türü. Örneğin:
    - `SIGINT` (klavyeden kesinti, genellikle `Ctrl+C` ile gönderilir),
    - `SIGTERM` (programı düzgün bir şekilde sonlandırmak için),
    - `SIGKILL` (programı anında sonlandırmak için, yakalanamaz).
- **`handler`**: Sinyal yakalandığında çağrılacak işlevin adresi.
    - `SIG_IGN`: Sinyali **yoksaymak** için kullanılır.
    - `SIG_DFL`: Sinyali varsayılan davranışla **işlemek** için kullanılır.
    - Kullanıcı tanımlı bir işlev adresi: Sinyal yakalandığında bu işlev çalıştırılır.
- Dönüş değeri: Önceki sinyal işleyicinin adresini döndürür.

**Sinyallerin Temel Amacı:**

Sinyallerin temel amacı, bir sürece belirli bir olayın meydana geldiğini bildirmektir. Bu olaylar şunlar olabilir:

- **Kullanıcı Etkileşimi:** Kullanıcının klavyeden bir tuşa basması (örneğin, Ctrl+C), bir fare tıklaması veya terminalden bir komut vermesi.
- **Hata Durumları:** Bir programda bir hata oluşması (örneğin, sıfıra bölme, geçersiz bellek erişimi).
- **Süreç Yönetimi:** Başka bir sürecin bir süreci sonlandırması veya bir alt sürecin durumunun değişmesi.
- **Zamanlayıcılar:** Belirli bir süre geçtikten sonra bir olayın tetiklenmesi.

**Sinyallerin Çalışma Mekanizması:**

1. **Olayın Meydana Gelmesi:** Bir olay gerçekleştiğinde (örneğin, kullanıcı Ctrl+C'ye bastığında), işletim sistemi ilgili sürece bir sinyal gönderir.
2. **Sinyalin Gönderilmesi:** İşletim sistemi, hedef sürecin süreç kontrol bloğuna (PCB) sinyal bilgisini ekler.
3. **Sürecin Kontrolü:** Süreç bir sonraki uygun zamanda (genellikle bir sistem çağrısından dönüşte veya bir kesme işlendikten sonra) bekleyen sinyalleri kontrol eder.
4. **Sinyal İşleyicisi (Signal Handler):** Eğer süreç, gelen sinyal için bir sinyal işleyicisi tanımlamışsa, bu işleyici fonksiyonu çalıştırılır. Sinyal işleyicisi, sinyale özgü bir eylemi gerçekleştirmek için tasarlanmış bir fonksiyondur.
5. **Varsayılan Davranış (Default Action):** Eğer süreç sinyal için bir işleyici tanımlamamışsa, işletim sisteminin o sinyal için tanımladığı varsayılan davranış uygulanır. Varsayılan davranışlar şunlar olabilir:
    - **Süreci Sonlandırma (Terminate):** Örneğin, `SIGSEGV` (geçersiz bellek erişimi) sinyalinin varsayılan davranışı süreci sonlandırmaktır.
    - **Süreci Durdurma (Stop):** Örneğin, `SIGSTOP` sinyali süreci durdurur.
    - **Süreci Devam Ettirme (Continue):** Örneğin, `SIGCONT` sinyali durdurulmuş bir süreci devam ettirir.
    - **Sinyali Yok Sayma (Ignore):** Bazı sinyaller varsayılan olarak yok sayılır.

**Örnekler ve Açıklamalar:**

- **Ctrl+C (`SIGINT`):** Kullanıcı terminalde çalışan bir süreci durdurmak için Ctrl+C tuşlarına bastığında, terminal sürücüsü çalışan sürece `SIGINT` sinyali gönderir. Genellikle bu sinyalin varsayılan davranışı süreci sonlandırmaktır. Ancak, süreç bu sinyal için bir işleyici tanımlayarak farklı bir eylem gerçekleştirebilir (örneğin, temizleme işlemleri yapıp sonra sonlanmak).
- **`kill` Komutu:** `kill` komutu, bir sürece belirli bir sinyal göndermek için kullanılır. Örneğin, `kill -9 <PID>` komutu, belirtilen PID'ye sahip sürece `SIGKILL` sinyali gönderir. `SIGKILL`, yakalanamayan ve bloke edilemeyen bir sinyaldir; bu nedenle süreci hemen sonlandırır.
- **Sıfıra Bölme (`SIGFPE`):** Bir programda sıfıra bölme gibi bir aritmetik hata oluştuğunda, işletim sistemi sürece `SIGFPE` sinyali gönderir. Bu sinyalin varsayılan davranışı genellikle süreci sonlandırmaktır.
- **Geçersiz Bellek Erişimi (`SIGSEGV`):** Bir program, kendisine ait olmayan bir bellek alanına erişmeye çalıştığında, işletim sistemi sürece `SIGSEGV` sinyali gönderir. Bu sinyalin varsayılan davranışı da süreci sonlandırmaktır.

**Özetle:**

Sinyaller, işletim sistemi ve süreçler arasında ve süreçlerin kendi aralarında asenkron iletişim kurmasını sağlayan güçlü bir mekanizmadır. Olaylara hızlı bir şekilde tepki vermeyi, hataları işlemeyi ve süreçleri yönetmeyi mümkün kılarlar. `signal.h` başlık dosyası ve ilgili sistem çağrıları (`signal`, `sigaction`, `kill` vb.) bu mekanizmayı programlarımızda kullanmamızı sağlar.

![mini](https://github.com/user-attachments/assets/93b76cc0-d2f8-4dda-86a0-c0f94bb9b27a)
Kaynak: https://42-cursus.gitbook.io/guide/rank-02/minitalk/understand-minitalk

Bu görsel, bir **client** (istemci) ve **server** (sunucu) arasında sinyallerle veri iletiminin nasıl gerçekleştiğini açıklıyor.

---

### **Client ve Server Arasında "Hello" Gönderimi**

1. **İletişim Başlangıcı**:
    - İstemci, `"Hello"` kelimesini server'a göndermek istiyor. Ancak `"Hello"` gibi bir string (metin) doğrudan sinyallerle gönderilemez.
    

---

1. **ASCII Dönüşümü**:
    - `"Hello"` stringindeki her karakter, ASCII tablosuna göre bir sayıya dönüştürülür:
        - `H -> 72`
        - `e -> 101`
        - `l -> 108`
        - `l -> 108`
        - `o -> 111`

---

1. **Binary (İkili) Dönüşüm**:
    - Bu ASCII değerleri, binary (ikili) formata çevrilir:
        - `H (72)` → `01001000`
        - `e (101)` → `01100101`
        - `l (108)` → `01101100`
        - `l (108)` → `01101100`
        - `o (111)` → `01101111`
    - Her karakterin ASCII değeri 8-bitlik binary veriye çevrildiği için toplamda:
        - 5 harf x 8 bit = **40 bit** sinyal iletilir.

---

1. **Sinyallerin Gönderimi**:
    - Bu binary veriler, **bit bit** (0 ve 1'ler şeklinde) sinyallerle server'a gönderilir.
    - Her 0 ve 1 bir sinyal dalgasını temsil eder:
        - Örneğin: `0, 1, 0, 0, 1, 0, 0, 0` şeklinde ilerler.
    - İstemci bu sinyalleri gönderirken, her bit birer birer server'a iletilir.

---

1. **Server Tarafında Alınan Veri**:
    - Server, gelen sinyalleri toplar ve binary veriye dönüştürür.
    - Binary veri tekrar ASCII'ye çevrilir:
        - `01001000` → `H`
        - `01100101` → `e`
        - `01101100` → `l`
        - `01101100` → `l`
        - `01101111` → `o`
    - Bu işlem sonucunda server, `"Hello"` stringini yeniden oluşturur.

---

### **Önemli Noktalar**

1. **Doğrudan String Gönderimi Mümkün Değil**:
    - `"Hello"` gibi bir string, sinyallerle doğrudan gönderilemez. Her harf önce sayıya, sonra binary (ikili) veriye çevrilir.
2. **Sinyal İletimi**:
    - Her bir 0 ve 1, bir sinyal olarak gönderilir. Sinyallerin tamamı server'a ulaştığında veri deşifre edilir (çözülür).
3. **Küçük Bir Hata Büyük Sorun Yaratır**:
    - Gönderilen sinyallerin sırası ya da bir bitin eksik olması, alınan veriyi tamamen bozabilir.

---

## **Sigaction**

**sigaction** yapısı aşağıdaki alanları içerir:

| **Üye Türü** | **Üye Adı** | **Açıklama** |
| --- | --- | --- |
| void (*) (int) | sa_işleyicisi | SIG_DFL, SIG_IGN ya da bir işleve ilişkin gösterge. |
| imza_t | sa_maskesi | Sinyal yakalama işlevinin yürütülmesi sırasında engellenecek ek sinyal kümesi. |
| INT | sa_işaretleri | Sinyalin davranışını etkileyen özel işaretler. |
| void (*) (int, siginfo_t *, void *) | sa_sigaction | Sinyal yakalama fonksiyonu. |

`sigaction`, POSIX uyumlu işletim sistemlerinde (Linux, Unix) sinyal işlemleri için kullanılan güçlü bir fonksiyondur. Bu fonksiyon, belirli bir sinyalin nasıl ele alınacağını tanımlamak için kullanılır. Eski `signal()` fonksiyonunun daha esnek ve güvenli bir alternatifi olarak önerilir.

---

### **Fonksiyon Tanımı**

```c

#include <signal.h>int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
```

- `signum`: İşlemek istediğiniz sinyal numarası (örn. `SIGINT`, `SIGTERM`, `SIGUSR1`).
- `act`: Yeni sinyal işleyici bilgilerini içeren `struct sigaction` yapısına bir işaretçi.
- `oldact`: Önceki sinyal işlemi hakkında bilgi almak için kullanılan yapı (isteğe bağlı, `NULL` verilebilir).

### **Dönüş Değeri**

Başarılı olduğunda `0`, hata durumunda `-1` döner ve `errno` güncellenir.

### **struct sigaction Yapısı**

```c
c
KopyalaDüzenle
struct sigaction {
    void (*sa_handler)(int);         // Sinyal işleyici (veya varsayılan işlemler)
    void (*sa_sigaction)(int, siginfo_t *, void *); // Alternatif işleyici
    sigset_t sa_mask;                // Bloklanacak sinyallerin seti
    int sa_flags;                    // Çeşitli bayraklar
};

```

### **Alanların Detaylı Açıklaması**

- **`sa_handler`**:
    
    Sinyal geldiğinde çağrılacak işlemci fonksiyonunun adresi. Aşağıdaki özel değerlere sahip olabilir:
    
    - `SIG_DFL`: Varsayılan işlem (örn. `SIGTERM` ile işlem sonlanır).
    - `SIG_IGN`: Sinyali yok say (örn. `SIGINT` sinyalini yok sayabilirsiniz).
    - 

### **Bayrakların Detaylı Açıklaması**

- **`SA_SIGINFO`**:
`sa_sigaction` işleyicisini aktif hale getirir. Böylece `siginfo_t` ile ek bilgi alınabilir.
- **`SA_RESTART`**:
Sistem çağrılarının kesilmesini engeller. Eğer sinyal geldiğinde `read()` veya `write()` gibi sistem çağrıları kesilirse, işlem otomatik olarak yeniden başlatılır.
- **`SA_NODEFER`**:
İşleyici çalışırken aynı sinyalin tekrar engellenmesini önler.
- **`SA_NOCLDWAIT`**:
`SIGCHLD` sinyali için çocuk işlemler sonlandığında zombiler oluşmasını engeller.

---

### **sigaction vs signal**

| Özellik | `sigaction` | `signal` |
| --- | --- | --- |
| Güvenlik | Daha güvenli | Daha az güvenli |
| Bayrak Desteği | Var | Yok |
| Esneklik | Yüksek | Düşük |
| Taşınabilirlik | Daha iyi | Daha kötü |
| Kullanım Amacı | Gelişmiş sinyal işlemleri | Basit sinyal işlemleri |

## Neden projeyi signal( ) ve usleep( ) kullanarak yapabilecekken sigaction( ) tercih ettik?

Ubuntu (Linux) ve Mac (macOS) bilgisayarlarının sinyal gönderiminde farklı davranmasının temel nedeni, işletim sistemlerinin çekirdek düzeyindeki mimarileri ve POSIX uyumluluğu ile ilgili farklılıklarıdır.

### 1. **Çekirdek Farklılıkları**

- **Ubuntu (Linux)**: Linux çekirdeği tamamen açık kaynak kodludur ve POSIX uyumuna büyük ölçüde odaklanır. Sinyal işleme (signals) mekanizması POSIX standartları ile uyumludur.
- **Mac (macOS)**: macOS, Unix tabanlı Darwin çekirdeği ile çalışır. Apple’ın çekirdeği olan XNU (eXperimental NeXTSTEP Kernel) BSD ve Mach bileşenlerini birleştirir. Bu durum sinyal mekanizmalarında bazı farklılıklara yol açabilir.

---

### 2. **Sinyal Kütüphanelerindeki Farklılıklar**

- Ubuntu'da **glibc (GNU C Library)** gibi kütüphaneler kullanılırken,
- macOS'ta **libSystem** ve **BSD API'leri** daha yoğun şekilde kullanılır.

Kütüphanelerdeki bu farklılık bazı sinyal işlemlerinin detaylarında (örneğin `sigaction`, `signal`) farklılıklara neden olabilir.

---

### 3. **Sinyal Öncelikleri ve Kernel İşlemleri**

Linux'ta bazı sinyaller daha düşük seviyede optimize edilmişken, macOS'ta kernel tasarımı gereği farklı işlem sıralamaları ve gecikmeler olabilir. Bu farklılık özellikle `SIGCHLD`, `SIGUSR1`, `SIGUSR2` gibi sinyallerin işlenmesinde ortaya çıkabilir.

---

### 4. **Sinyal Varsayılan Davranışları**

Ubuntu ile macOS arasında bazı sinyallerin varsayılan davranışlarında farklılıklar görülebilir:

- `SIGKILL` ve `SIGSTOP`: Her iki sistemde de benzer şekilde çalışır.
- Ancak `SIGPIPE`, macOS'ta bazı socket işlemlerinde farklı davranış gösterebilir.

---

### 5. **Kernel Politikaları**

macOS'un güvenlik politikaları gereği sinyal gönderim ve yakalama işlemlerine bazı kısıtlamalar getirilmiştir. Özellikle sistem süreçlerine sinyal gönderme gibi durumlarda daha katı kurallar vardır.

---

### Özetle

Bu farklılıklar Linux ve macOS'un farklı kernel yapılarına, sinyal kütüphanelerine ve güvenlik politikalarına dayanmaktadır. Eğer bir sinyal işleminin her iki sistemde aynı çalışmasını istiyorsanız `POSIX` uyumlu kütüphaneler ve `sigaction` fonksiyonunu tercih etmek en iyi yaklaşımdır.

## `volatile sig_atomic_t` kullanımı

- `volatile`, derleyiciye bu değişkenin değerinin her zaman bellekten okunması gerektiğini bildirir.
- Normalde derleyiciler optimizasyon yaparken bir değişkenin değerini işlemci kayıtlarına alabilir ve sürekli bellekten okumayı engeller.

**Neden Kullanılır?**

Sinyal işlemlerinde veya donanım tarafından güncellenen değişkenlerde `volatile` kullanımı gereklidir çünkü bu tür değişkenlerin değerleri beklenmedik zamanlarda değişebilir.

Daha detaylı değinecek olursak;

C derleyicileri, kodun daha verimli çalışması için değişkenlerin bellekten değil, işlemcinin register (kayıt) belleğinden okunmasını tercih eder. Eğer bir değişken bellekte değişse bile derleyici bunu görmez ve önceki değeri kullanmaya devam edebilir.

Bu durum özellikle dışsal olaylara (sinyaller, donanım kesmeleri) bağlı değişkenlerde tehlikeli sonuçlar doğurur. Çünkü sinyaller işletim sisteminin kontrolünde çalışır ve programın akışı dışında herhangi bir anda değişkeni güncelleyebilir.

### **`sig_atomic_t` Nedir?**

- `sig_atomic_t`, POSIX standardına uygun bir türdür ve sinyal işlemlerinde kullanılmak üzere tasarlanmıştır.
- Genellikle `int` veya `char` boyutunda bir türdür, ama bu işletim sistemine bağlı olarak değişebilir.
- En önemli özelliği, **atomik** (kesilmeden güvenli) bir okuma/yazma işlemi garanti etmesidir.

**Neden Kullanılır?**

Sinyal işlemcileri (`signal handlers`) sırasında sinyal işlenirken değişken güncellenirse işlem yarıda kesilebilir. `sig_atomic_t`, bu güncellemelerin atomik (kesintiye uğramadan) gerçekleşmesini sağlar.

## **Global Değişken Nedir?**

Global değişkenler, bir programın tüm fonksiyonları tarafından erişilebilen ve genellikle dosya veya programın başında tanımlanan değişkenlerdir. Bunlar **bloklar (fonksiyon, döngü gibi yapılar)** dışında tanımlanır ve **tüm program boyunca geçerlidir**.

### **Global Değişkenlerin Özellikleri**

1. **Blok Dışında Tanımlanır:** Herhangi bir fonksiyonun dışında, genellikle dosyanın başında tanımlanır.
2. **Tüm Fonksiyonlardan Erişilebilir:** Aynı dosya içindeki tüm fonksiyonlar bu değişkeni kullanabilir.
3. **Bellekte Kalıcıdır:** Program sonlanana kadar bellekte saklanır.
4. **Varsayılan Değer:** Global değişkenler tanımlandığında otomatik olarak `0` ile başlatılır (statik ömürlü olduğu için).
